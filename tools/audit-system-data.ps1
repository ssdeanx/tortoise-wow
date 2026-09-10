param([Parameter(Mandatory)][string]$QueryToolPath, [Parameter(Mandatory)][string]$SpellDbcPath)
$ErrorActionPreference='Stop'
$repo=Split-Path $PSScriptRoot -Parent
$out=Join-Path $repo 'docs/core-audit'
$auraDefines=Get-Content -LiteralPath (Join-Path $repo 'src/game/Spells/SpellAuraDefines.h') -Raw
$auraLimitMatch=[regex]::Match($auraDefines,'\bTOTAL_AURAS\s*=\s*(\d+)')
if(!$auraLimitMatch.Success){throw 'Cannot resolve native TOTAL_AURAS'}
$auraLimit=[uint32]$auraLimitMatch.Groups[1].Value
function Save($name,$value) { [IO.File]::WriteAllText((Join-Path $out $name), (ConvertTo-Json -InputObject $value -Depth 12), [Text.UTF8Encoding]::new($false)) }
# Read the native WDBC ID column. This is read-only; no client/server files modified.
$bytes=[IO.File]::ReadAllBytes((Resolve-Path $SpellDbcPath))
if([Text.Encoding]::ASCII.GetString($bytes,0,4) -ne 'WDBC'){throw 'Not a WDBC file'}
$count=[BitConverter]::ToUInt32($bytes,4); $fields=[BitConverter]::ToUInt32($bytes,8)
$size=[BitConverter]::ToUInt32($bytes,12); $strings=[BitConverter]::ToUInt32($bytes,16)
if($size -ne $fields*4 -or 20L+[long]$size*$count+$strings -ne $bytes.Length){throw 'Unexpected DBC record layout'}
$spells=[Collections.Generic.HashSet[uint32]]::new()
for($i=0;$i -lt $count;$i++){ $null=$spells.Add([BitConverter]::ToUInt32($bytes,20+$i*$size)) }
# Offsets are the 1.12 Spellfmt/SpellDbcEntry contract, not the unrelated
# multi-expansion SQL `spell` table. Refuse another client layout.
if($fields -ne 173){throw "Expected 173-field native Spell.dbc, got $fields"}
$dispatchIssues=@(); $learnIssues=@(); $effectCounts=@{}
for($i=0;$i -lt $count;$i++){
    $offset=20+$i*$size; $id=[BitConverter]::ToUInt32($bytes,$offset)
    for($slot=0;$slot -lt 3;$slot++){
        $effect=[BitConverter]::ToUInt32($bytes,$offset+4*(61+$slot))
        $aura=[BitConverter]::ToUInt32($bytes,$offset+4*(91+$slot))
        $trigger=[BitConverter]::ToUInt32($bytes,$offset+4*(109+$slot))
        if(!$effectCounts.ContainsKey([int]$effect)){$effectCounts[[int]$effect]=0}; $effectCounts[[int]$effect]++
        if($effect -ge 135 -or ($effect -and $aura -ge $auraLimit)){$dispatchIssues+=[pscustomobject]@{spell=$id;slot=$slot;effect=$effect;aura=$aura}}
        if($effect -in @(36,57) -and (!$trigger -or !$spells.Contains($trigger))){$learnIssues+=[pscustomobject]@{spell=$id;slot=$slot;effect=$effect;learn_spell=$trigger}}
    }
}
Save 'systems-dbc-dispatch-gaps.json' $dispatchIssues
Save 'systems-dbc-learn-gaps.json' $learnIssues
Save 'systems-dbc-effect-counts.json' @($effectCounts.Keys | Sort-Object | ForEach-Object { [pscustomobject]@{effect=$_;slots=$effectCounts[$_]} })
$queries=[ordered]@{
    counts="SELECT JSON_OBJECT('creatures',(SELECT COUNT(*) FROM creature),'templates',(SELECT COUNT(*) FROM creature_template),'quests',(SELECT COUNT(*) FROM quest_template),'events',(SELECT COUNT(*) FROM game_event),'pools',(SELECT COUNT(*) FROM pool_template),'skills',(SELECT COUNT(*) FROM skill_line_ability),'spell_chains',(SELECT COUNT(*) FROM spell_chain))"
    skill_refs="SELECT JSON_OBJECT('kind','skill','owner',id,'spell',spell_id,'class_mask',class_mask,'race_mask',race_mask,'skill',skill_id) FROM skill_line_ability"
    chain_refs="SELECT JSON_OBJECT('id',spell_id,'previous',prev_spell,'first',first_spell,'required',req_spell) FROM spell_chain"
    learn_refs="SELECT JSON_OBJECT('owner',entry,'spell',SpellID) FROM spell_learn_spell"
    all_creature_spell_refs=(1..8 | ForEach-Object { "SELECT JSON_OBJECT('kind','creature_spells','owner',entry,'slot',$_,'spell',spellId_$_) FROM creature_spells WHERE spellId_$_<>0" }) -join ' UNION ALL '
    event_gaps="SELECT JSON_OBJECT('kind','event_creature','owner',e.guid,'ref',e.event) FROM game_event_creature e LEFT JOIN creature c ON c.guid=e.guid LEFT JOIN game_event g ON g.entry=ABS(e.event) WHERE c.guid IS NULL OR g.entry IS NULL UNION ALL SELECT JSON_OBJECT('kind','event_gameobject','owner',e.guid,'ref',e.event) FROM game_event_gameobject e LEFT JOIN gameobject c ON c.guid=e.guid LEFT JOIN game_event g ON g.entry=ABS(e.event) WHERE c.guid IS NULL OR g.entry IS NULL"
    pool_gaps="SELECT JSON_OBJECT('kind','pool_creature','owner',e.guid,'ref',e.pool_entry) FROM pool_creature e LEFT JOIN creature c ON c.guid=e.guid LEFT JOIN pool_template g ON g.entry=e.pool_entry WHERE c.guid IS NULL OR g.entry IS NULL UNION ALL SELECT JSON_OBJECT('kind','pool_gameobject','owner',e.guid,'ref',e.pool_entry) FROM pool_gameobject e LEFT JOIN gameobject c ON c.guid=e.guid LEFT JOIN pool_template g ON g.entry=e.pool_entry WHERE c.guid IS NULL OR g.entry IS NULL"
    quest_links="SELECT JSON_OBJECT('entry',entry,'previous',PrevQuestId,'next',NextQuestId,'chain',NextQuestInChain) FROM quest_template"
    reference_edges="SELECT JSON_OBJECT('owner',entry,'ref',-mincountOrRef) FROM reference_loot_template WHERE mincountOrRef<0"
    reference_ids="SELECT DISTINCT JSON_OBJECT('id',entry) FROM reference_loot_template"
    event_schedule="SELECT JSON_OBJECT('entry',entry,'start',start_time,'end',end_time,'occurrence',occurence,'length',length,'hardcoded',hardcoded,'disabled',disabled) FROM game_event"
    route_destinations="SELECT JSON_OBJECT('kind','area_trigger','owner',a.id,'map',a.target_map) FROM areatrigger_teleport a LEFT JOIN map_template m ON m.entry=a.target_map WHERE m.entry IS NULL UNION ALL SELECT JSON_OBJECT('kind','spell_target','owner',a.id,'map',a.target_map) FROM spell_target_position a LEFT JOIN map_template m ON m.entry=a.target_map WHERE m.entry IS NULL"
}
$tables=@('creature_loot_template','gameobject_loot_template','item_loot_template','reference_loot_template','pickpocketing_loot_template','skinning_loot_template','fishing_loot_template','disenchant_loot_template')
foreach($table in $tables){
    $queries["loot_$table"]="SELECT JSON_OBJECT('kind','missing_reference','table','$table','owner',l.entry,'ref',-l.mincountOrRef) FROM $table l LEFT JOIN (SELECT DISTINCT entry FROM reference_loot_template) r ON r.entry=-l.mincountOrRef WHERE l.mincountOrRef<0 AND r.entry IS NULL UNION ALL SELECT JSON_OBJECT('kind','missing_item','table','$table','owner',l.entry,'ref',l.item) FROM $table l LEFT JOIN item_template i ON i.entry=l.item WHERE l.mincountOrRef>=0 AND l.item<>0 AND i.entry IS NULL"
}
$questItems=@()
foreach($field in @('ReqItemId','ReqSourceId','RewItemId','RewChoiceItemId')) {
    $slots=if($field -eq 'RewChoiceItemId'){6}else{4}
    foreach($slot in 1..$slots) {
        $column="$field$slot"
        $questItems+="SELECT JSON_OBJECT('quest',q.entry,'field','$column','item',q.$column) FROM quest_template q LEFT JOIN item_template i ON i.entry=q.$column WHERE q.$column<>0 AND i.entry IS NULL"
    }
}
$queries['quest_item_gaps']=$questItems -join ' UNION ALL '
$questActors=@()
foreach($slot in 1..4) {
    $column="ReqCreatureOrGOId$slot"
    $questActors+="SELECT JSON_OBJECT('quest',q.entry,'field','$column','actor',q.$column) FROM quest_template q LEFT JOIN creature_template c ON c.entry=q.$column LEFT JOIN gameobject_template g ON g.entry=-q.$column WHERE (q.$column>0 AND c.entry IS NULL) OR (q.$column<0 AND g.entry IS NULL)"
}
$queries['quest_actor_gaps']=$questActors -join ' UNION ALL '
$queries['trainer_refs']="SELECT JSON_OBJECT('kind','trainer','owner',entry,'spell',spell) FROM npc_trainer UNION ALL SELECT JSON_OBJECT('kind','trainer_template','owner',entry,'spell',spell) FROM npc_trainer_template"
$queries['quest_spell_refs']="SELECT JSON_OBJECT('kind','quest_reward','owner',entry,'spell',RewSpell) FROM quest_template WHERE RewSpell<>0 UNION ALL SELECT JSON_OBJECT('kind','quest_reward_cast','owner',entry,'spell',RewSpellCast) FROM quest_template WHERE RewSpellCast<>0"
$queries['template_ranges']="SELECT JSON_OBJECT('entry',entry,'name',name,'health_min',health_min,'health_max',health_max,'dmg_min',dmg_min,'dmg_max',dmg_max) FROM creature_template WHERE health_min>health_max OR dmg_min>dmg_max OR level_min>level_max"
$results=@{}
foreach($name in $queries.Keys){
    $sql=$queries[$name]
    if($sql -notmatch '^SELECT\b' -or $sql.Contains(';')){throw 'Refusing non-read-only query'}
    # Session-free optimizer hint: unsupported servers can ignore it.
    $sql=$sql -replace '^SELECT ', 'SELECT /*+ MAX_EXECUTION_TIME(10000) */ '
    $rows=@(& $QueryToolPath -Database tw_world -Query $sql | ForEach-Object {$_ | ConvertFrom-Json})
    $results[$name]=$rows; Save "systems-$name.json" $rows
    Write-Output "$name=$($rows.Count)"
}
$spellGaps=@()
foreach($name in @('skill_refs','learn_refs','all_creature_spell_refs','trainer_refs','quest_spell_refs')){
    $spellGaps+=@($results[$name] | Where-Object { $_.spell -and !$spells.Contains([uint32]$_.spell) })
}
Save 'systems-spell-reference-gaps.json' $spellGaps
$questIds=[Collections.Generic.HashSet[int]]::new()
foreach($q in $results.quest_links){$null=$questIds.Add([int]$q.entry)}
$questGaps=@(foreach($q in $results.quest_links){foreach($field in @('previous','next','chain')){if($q.$field -and !$questIds.Contains([Math]::Abs([int]$q.$field))){[pscustomobject]@{quest=$q.entry;field=$field;target=$q.$field}}}})
Save 'systems-quest-link-gaps.json' $questGaps
$chainGaps=@(foreach($chain in $results.chain_refs){foreach($field in @('id','previous','first','required')){if($chain.$field -and !$spells.Contains([uint32]$chain.$field)){[pscustomobject]@{spell=$chain.id;field=$field;target=$chain.$field}}}})
Save 'systems-chain-spell-gaps.json' $chainGaps
# Directed reference-loot cycle detection, with no recursion-depth dependency.
$edges=@{}; foreach($e in $results.reference_edges){if(!$edges.ContainsKey([int]$e.owner)){$edges[[int]$e.owner]=@()}; $edges[[int]$e.owner]+=[int]$e.ref}
$cycles=[Collections.Generic.HashSet[string]]::new()
foreach($root in $edges.Keys){
    $stack=[Collections.Generic.Stack[object]]::new(); $stack.Push(@([int]$root))
    while($stack.Count){$path=@($stack.Pop()); $last=[int]$path[-1]; foreach($next in $edges[$last]){
        if($next -in $path){$null=$cycles.Add(($path -join '->')+'->'+$next)} else {$stack.Push(@($path)+$next)}
    }}
}
Save 'systems-loot-reference-cycles.json' @($cycles)
$summary=[ordered]@{captured_utc=[DateTime]::UtcNow.ToString('o');source_commit=(& git -c "safe.directory=$repo" -C $repo rev-parse HEAD);spell_dbc_sha256=(Get-FileHash $SpellDbcPath).Hash;spell_dbc_records=$count;counts=$results.counts;spell_reference_gaps=$spellGaps.Count;quest_link_gaps=$questGaps.Count;chain_spell_gaps=$chainGaps.Count;spell_dispatch_gaps=$dispatchIssues.Count;learn_effect_gaps=$learnIssues.Count;loot_cycles=$cycles.Count;event_reference_gaps=$results.event_gaps.Count;pool_reference_gaps=$results.pool_gaps.Count;missing_route_maps=$results.route_destinations.Count;note='Read-only structural checks, not gameplay certification; spell IDs checked against supplied Spell.dbc, not unused SQL spell tables.'}
Save 'systems-summary.json' $summary
$summary | ConvertTo-Json -Depth 5
