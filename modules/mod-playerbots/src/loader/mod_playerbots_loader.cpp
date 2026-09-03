/*
 * mod-playerbots — module entry point.
 *
 * The name must be Addmod_playerbotsScripts(), derived from the directory name
 * "mod-playerbots": the generated ModulesLoader.cpp declares and calls one such
 * function per module, and the link fails without it.
 *
 * DELIBERATELY EMPTY. This module does not register through the module loader,
 * and moving it there would be a behaviour change, not a tidy-up. The bots come
 * up through World::InitPlayerbotsAtStartup() (HostHooks.cpp), which the core
 * calls at a point chosen for a reason the comment there spells out: the hook
 * objects must be registered AFTER the ScriptRegistry containers are
 * constructed, so a file-scope instance - or an earlier loader call - would run
 * too soon. That bootstrap already does sPlayerbotAIConfig.Initialize() and
 * AddSC_playerbot_hooks(); doing it twice, or earlier, is not an improvement.
 *
 * So: the symbol exists to satisfy the loader, and the real wiring stays where
 * it was proven to work.
 */

void Addmod_playerbotsScripts()
{
}
