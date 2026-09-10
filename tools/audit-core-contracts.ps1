param(
    [string]$QueryToolPath,
    [switch]$RefreshDatabase
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
$out = Join-Path $repo 'docs/core-audit'
New-Item -ItemType Directory -Path $out -Force | Out-Null
function Save-Json($Name, $Value) {
    # Generated audit output only; no server config or SQL mutation.
    $json = ConvertTo-Json -InputObject $Value -Depth 15
    [IO.File]::WriteAllText((Join-Path $out $Name), $json, [Text.UTF8Encoding]::new($false))
}
if ($RefreshDatabase) {
    if (!$QueryToolPath) { throw 'Pass the existing local read-only query adapter explicitly.' }
    $queries = [ordered]@{
        maps = @'
SELECT /*+ MAX_EXECUTION_TIME(10000) */ JSON_OBJECT('entry',m.entry,'name',m.map_name,'type',m.map_type,'script',m.script_name,'creature_spawns',COALESCE(c.n,0),'gameobject_spawns',COALESCE(g.n,0)) FROM map_template m LEFT JOIN (SELECT map,COUNT(*) n FROM creature GROUP BY map) c ON c.map=m.entry LEFT JOIN (SELECT map,COUNT(*) n FROM gameobject GROUP BY map) g ON g.map=m.entry ORDER BY m.entry
'@
        templates = @'
SELECT /*+ MAX_EXECUTION_TIME(10000) */ JSON_OBJECT('entry',c.entry,'name',c.name,'rank',c.rank,'script',c.script_name,'ai',c.ai_name,'level_min',c.level_min,'level_max',c.level_max,'health_min',c.health_min,'health_max',c.health_max,'damage_min',c.dmg_min,'damage_max',c.dmg_max,'damage_multiplier',c.dmg_multiplier,'loot_id',c.loot_id,'loot_rows',COALESCE(l.n,0),'spell_list_id',c.spell_list_id,'spell_list_exists',IF(s.entry IS NULL,0,1),'event_rows',COALESCE(e.n,0),'movement_type',c.movement_type) FROM creature_template c LEFT JOIN (SELECT entry,COUNT(*) n FROM creature_loot_template GROUP BY entry) l ON l.entry=c.loot_id LEFT JOIN creature_spells s ON s.entry=c.spell_list_id LEFT JOIN (SELECT creature_id,COUNT(*) n FROM creature_ai_events GROUP BY creature_id) e ON e.creature_id=c.entry ORDER BY c.entry
'@
        spawn_refs = @'
SELECT /*+ MAX_EXECUTION_TIME(10000) */ JSON_OBJECT('map',map,'entry',entry,'slots',COUNT(*),'minimum_health_percent',MIN(health_percent),'maximum_health_percent',MAX(health_percent)) FROM (SELECT map,id entry,health_percent FROM creature UNION ALL SELECT map,id2,health_percent FROM creature WHERE id2<>0 UNION ALL SELECT map,id3,health_percent FROM creature WHERE id3<>0 UNION ALL SELECT map,id4,health_percent FROM creature WHERE id4<>0) c WHERE entry<>0 GROUP BY map,entry ORDER BY map,entry
'@
        objects = @'
SELECT /*+ MAX_EXECUTION_TIME(10000) */ JSON_OBJECT('map',g.map,'entry',g.id,'spawns',COUNT(*),'name',t.name,'type',t.type,'script',t.script_name,'data0',t.data0,'data1',t.data1,'data2',t.data2,'data3',t.data3,'template_exists',IF(t.entry IS NULL,0,1)) FROM gameobject g LEFT JOIN gameobject_template t ON t.entry=g.id GROUP BY g.map,g.id,t.entry,t.name,t.type,t.script_name,t.data0,t.data1,t.data2,t.data3 ORDER BY g.map,g.id
'@
        event_bindings = @'
SELECT JSON_OBJECT('kind','event','entry',id,'script',script_name) FROM scripted_event_id UNION ALL SELECT JSON_OBJECT('kind','area_trigger','entry',entry,'script',script_name) FROM scripted_areatrigger
'@
        loot_gaps = @'
SELECT /*+ MAX_EXECUTION_TIME(10000) */ JSON_OBJECT('kind','missing_item','store',l.entry,'item',l.item) FROM creature_loot_template l LEFT JOIN item_template i ON i.entry=l.item WHERE l.mincountOrRef>=0 AND l.item<>0 AND i.entry IS NULL UNION ALL SELECT JSON_OBJECT('kind','missing_reference','store',l.entry,'reference',-l.mincountOrRef) FROM creature_loot_template l LEFT JOIN (SELECT DISTINCT entry FROM reference_loot_template) r ON r.entry=-l.mincountOrRef WHERE l.mincountOrRef<0 AND r.entry IS NULL
'@
    }
    foreach ($name in $queries.Keys) {
        $sql = $queries[$name].Trim()
        if ($sql -notmatch '^SELECT\b' -or $sql.Contains(';')) { throw "Non-read query refused: $name" }
        $rows = @(& $QueryToolPath -Database tw_world -Query $sql | ForEach-Object { $_ | ConvertFrom-Json })
        Save-Json "db-$name.json" $rows
        Write-Output "$name rows=$($rows.Count)"
    }
    Save-Json 'db-capture.json' @{ captured_utc = [DateTime]::UtcNow.ToString('o'); schema='tw_world'; consistent_transaction=$false; note='Sequential read-only SELECT snapshots; no account/character data.' }
}
function Read-Json($Name) { Get-Content -LiteralPath (Join-Path $out $Name) -Raw | ConvertFrom-Json }
$maps = @(Read-Json 'db-maps.json')
$templates = @(Read-Json 'db-templates.json')
$refs = @(Read-Json 'db-spawn_refs.json')
$objects = @(Read-Json 'db-objects.json')
$mapById = @{}; foreach ($m in $maps) { $mapById[[int]$m.entry] = $m }
$templateById = @{}; foreach ($t in $templates) { $templateById[[int]$t.entry] = $t }
# Strip comments while retaining strings and line breaks. This is lexical
# evidence, not a C++ parser, preprocessor or runtime registration inspection.
function Strip-Comments([string]$Text) {
    [regex]::Replace($Text, '"(?:\\.|[^"\\])*"|''(?:\\.|[^''\\])*''|//[^\r\n]*|/\*[\s\S]*?\*/', {
        param($m)
        if ($m.Value.StartsWith('//') -or $m.Value.StartsWith('/*')) { return [regex]::Replace($m.Value, '[^\r\n]', ' ') }
        $m.Value
    })
}
$loader = Strip-Comments (Get-Content "$repo/src/scripts/ScriptLoader.cpp" -Raw)
$cmake = Get-Content "$repo/src/scripts/CMakeLists.txt" -Raw
$gameCmake = Get-Content "$repo/src/game/CMakeLists.txt" -Raw
$registrations = [Collections.Generic.List[object]]::new()
$sourceRows = [Collections.Generic.List[object]]::new()
$capabilities = [ordered]@{
    ai_update='\bUpdateAI\s*\('; spells='\b(?:DoCast\w*|CastSpell)\s*\('; movement='\b(?:MovementInform|MovePoint|MoveWaypoint|MoveChase|MoveFollow|MoveJump|MoveSplineInit)\b';
    summons='\b(?:SummonCreature|JustSummoned|SummonedCreature\w*)\b'; death_reset='\b(?:JustDied|Reset|EnterEvadeMode|JustReachedHome)\s*\(';
    instance_state='\b(?:SetData|GetData|SetData64|GetData64|SetBossState)\s*\('; doors='\b(?:DoUseDoorOrButton|UseDoorOrButton|SetGoState|HandleGameObject)\s*\(';
    timers='\b(?:EventMap|EventProcessor|UpdateTimers|ScheduleEvent|ui\w*Timer|m_\w*Timer)\b'; threat='\b(?:GetThreatManager|SelectAttackingTarget|ResetThreat|AttackStart)\s*\(';
    async='\b(?:std::thread|std::async|processWorkload|AddAsyncTask)\b'
}
$paths = @(& rg --files "$repo/src/scripts" "$repo/src/game" "$repo/src/shared" "$repo/src/modules" "$repo/modules" -g '*.cpp' -g '*.h' -g '*.hpp')
foreach ($path in $paths) {
    $relative = [IO.Path]::GetRelativePath($repo,$path).Replace('\','/')
    $text = Strip-Comments (Get-Content -LiteralPath $path -Raw)
    # Some script registrations live in src/game (notably GenericSpellAI).
    # Module/shared builds need their own CMake/config review: null != excluded.
    $listed = $null
    if ($relative.StartsWith('src/scripts/')) { $listed = $cmake.Contains($relative.Substring('src/scripts/'.Length)) }
    elseif ($relative.StartsWith('src/game/')) { $listed = $gameCmake.Contains($relative.Substring('src/game/'.Length)) }
    $entryPoints = @([regex]::Matches($text,'\bvoid\s+(AddSC_\w+)\s*\([^)]*\)\s*\{') | ForEach-Object { $_.Groups[1].Value })
    $called = @($entryPoints | Where-Object { $loader -match ('(?m)^\s*' + [regex]::Escape($_) + '\s*\(\s*\)\s*;') })
    $signals = @($capabilities.Keys | Where-Object { $text -match $capabilities[$_] })
    $row = [pscustomobject]@{file=$relative; cmake_listed=$listed; addsc=$entryPoints; loader_calls=$called; dependency_signals=$signals; sha256=(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash}
    $sourceRows.Add($row)
    foreach ($match in [regex]::Matches($text,'->Name\s*=\s*"([^"]+)"')) {
        $registrations.Add([pscustomobject]@{name=$match.Groups[1].Value;file=$relative;line=1+([regex]::Matches($text.Substring(0,$match.Index),"`n").Count);cmake_listed=$listed;file_loader_call_found=($called.Count -gt 0);dependency_signals=$signals})
    }
}
$byName = @{}; foreach ($reg in $registrations) { if (!$byName.ContainsKey($reg.name)) { $byName[$reg.name]=@() }; $byName[$reg.name]+=$reg }
$encounters = [Collections.Generic.List[object]]::new()
foreach ($ref in $refs) {
    $m = $mapById[[int]$ref.map]; $t = $templateById[[int]$ref.entry]
    if (!$m -or !$t) { continue }
    if ($m.type -notin @(1,2) -and $t.rank -ne 3) { continue }
    $evidence = if ($t.script -and $byName.ContainsKey($t.script)) { @($byName[$t.script]) } else { @() }
    $issues = @()
    if ($t.script -and !$evidence.Count) { $issues+='script_registration_not_found_by_scanner' }
    if ($evidence.Count -and !@($evidence | Where-Object { $_.cmake_listed -and $_.file_loader_call_found }).Count) { $issues+='registration_build_or_loader_needs_review' }
    if ($t.ai -eq 'EventAI' -and !$t.event_rows) { $issues+='EventAI_no_event_rows' }
    if ($t.spell_list_id -and !$t.spell_list_exists) { $issues+='missing_spell_list' }
    if ($t.loot_id -and !$t.loot_rows) { $issues+='missing_loot_store' }
    if ($t.health_min -gt $t.health_max -or $t.damage_min -gt $t.damage_max) { $issues+='reversed_stat_range' }
    $encounters.Add([pscustomobject]@{
        map=$m.entry;map_name=$m.name;map_type=$m.type;entry=$t.entry;name=$t.name;rank=$t.rank;spawn_slots=$ref.slots;
        role='unclassified: rank/script name is not a complete boss taxonomy';script=$t.script;ai=$t.ai;spell_list=$t.spell_list_id;event_rows=$t.event_rows;
        loot_id=$t.loot_id;loot_rows=$t.loot_rows;health_min=$t.health_min;health_max=$t.health_max;damage_min=$t.damage_min;damage_max=$t.damage_max;damage_multiplier=$t.damage_multiplier;
        source_evidence=$evidence;flags=$issues;architecture= $(if ($m.type -in @(1,2)) {'Instance: outside continent-only creature throttling; shared movement/protocol/ownership still apply'} else {'World-boss rank: protected by background policy once selected; activation/dependencies still need validation'});
        gameplay_validation='not performed'
    })
}
$bindings = @($maps | Where-Object script | ForEach-Object { [pscustomobject]@{kind='map';map=$_.entry;entry=$_.entry;script=$_.script} })
$bindings += @($objects | Where-Object { $_.script } | ForEach-Object { [pscustomobject]@{kind='gameobject';map=$_.map;entry=$_.entry;script=$_.script} })
$bindings += @(Read-Json 'db-event_bindings.json' | ForEach-Object { [pscustomobject]@{kind=$_.kind;map=$null;entry=$_.entry;script=$_.script} })
$bindingAudit = @($bindings | ForEach-Object { [pscustomobject]@{kind=$_.kind;map=$_.map;entry=$_.entry;script=$_.script;source_evidence=@($byName[$_.script] | Where-Object { $_ });status=$(if ($byName.ContainsKey($_.script)) {'source registration candidate found; validate entrypoint/build'} else {'registration not found by scanner; manual resolution required'})} })
Save-Json 'source-index.json' @($sourceRows)
Save-Json 'script-registration-index.json' @($registrations)
Save-Json 'encounter-creature-matrix.json' @($encounters)
Save-Json 'instance-object-binding-matrix.json' $bindingAudit
Save-Json 'source-only-registration-candidates.json' @($registrations | Where-Object { $_.file -like 'src/scripts/dungeons/*' -and $_.name -notin $templates.script -and $_.name -notin $bindings.script })
$summary = [ordered]@{
    source_commit=(& git -c "safe.directory=$repo" -C $repo rev-parse HEAD); generated_utc=[DateTime]::UtcNow.ToString('o');
    maps=$maps.Count; instance_maps=@($maps | Where-Object type -In @(1,2)).Count;
    creature_templates=$templates.Count; map_template_references=$refs.Count; source_files=$sourceRows.Count;
    dungeon_source_files=@($sourceRows | Where-Object file -Like 'src/scripts/dungeons/*').Count;
    registrations=$registrations.Count; encounter_map_template_rows=$encounters.Count;
    flagged_encounter_rows=@($encounters | Where-Object { $_.flags.Count }).Count;
    missing_spawn_templates=@($refs | Where-Object { !$templateById.ContainsKey([int]$_.entry) }).Count;
    missing_spawn_maps=@($refs | Where-Object { !$mapById.ContainsKey([int]$_.map) }).Count;
    no_runtime_validation=$true; scanner_limit='Lexical dependencies and literal legacy registrations only; not proof of all dynamic summons, preprocessing, runtime registrations or encounter completeness.'
}
Save-Json 'summary.json' $summary
$coverage = @($maps | Where-Object type -In @(1,2) | ForEach-Object {
    $m = $_
    $rows = @($encounters | Where-Object map -EQ $m.entry)
    [pscustomobject]@{
        map=$m.entry; name=$m.name; type=$m.type; instance_script=$m.script;
        creature_spawn_rows=$m.creature_spawns; gameobject_spawn_rows=$m.gameobject_spawns;
        distinct_spawn_template_references=$rows.Count;
        cpp_bound_templates=@($rows | Where-Object script).Count;
        eventai_named_templates=@($rows | Where-Object ai -EQ 'EventAI').Count;
        review_flags=@($rows | Where-Object { $_.flags.Count }).Count;
        validation='Inventory only; not an expected-content or gameplay completeness assertion'
    }
})
Save-Json 'map-coverage.json' $coverage
$lines = [Collections.Generic.List[string]]::new()
$lines.Add('# Dungeon and raid coverage inventory')
$lines.Add('')
$lines.Add('Generated by `tools/audit-core-contracts.ps1`. Source: `' + $summary.source_commit + '`. See `db-capture.json` for the independently captured database timestamp.')
$lines.Add('')
$lines.Add('Every map with database map_type 1 or 2 is listed, including test/unused candidates. Spawn rows are stored definitions, not proof of simultaneous live population. Templates count all four creature entry slots; dynamically summoned actors need separate lifecycle tracing. Blank instance scripts and review flags are not automatic failures. No boss fight, door progression, loot roll or respawn was exercised.')
$lines.Add('')
$lines.Add('| Map | Name | Creature rows | Object rows | Template refs | C++ named | EventAI named | Flagged refs | Instance script |')
$lines.Add('| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |')
foreach ($m in $coverage) {
    $lines.Add("| $($m.map) | $($m.name) | $($m.creature_spawn_rows) | $($m.gameobject_spawn_rows) | $($m.distinct_spawn_template_references) | $($m.cpp_bound_templates) | $($m.eventai_named_templates) | $($m.review_flags) | $($m.instance_script) |")
}
$lines.Add('')
$lines.Add('## Dungeon source directories')
$lines.Add('')
$lines.Add('All directories below were lexically indexed. This is a source discovery index, not a claim of line-by-line semantic or runtime validation. Per-file hashes, loader candidates and dependency signals are in `source-index.json`.')
$lines.Add('')
foreach ($group in ($sourceRows | Where-Object file -Like 'src/scripts/dungeons/*' | Group-Object { ($_.file -split '/')[3] } | Sort-Object Name)) {
    $lines.Add('- `' + $group.Name + '`: ' + $group.Count + ' source/header files')
}
[IO.File]::WriteAllText((Join-Path $out 'COVERAGE.md'), ($lines -join "`n") + "`n", [Text.UTF8Encoding]::new($false))
$summary | ConvertTo-Json -Depth 3
