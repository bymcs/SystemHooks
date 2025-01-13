#include <Windows.h>
#include "../include/HookManager.h"
#include "../include/DiscordRPCHook.h"
#include "../include/MacAddressHook.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);
            
            // Debug modunu aktifleştir
            HookManager::EnableDebug(true);
            HookManager::DebugLog("[*] DLL_PROCESS_ATTACH\n");
            
            auto& hookManager = HookManager::GetInstance();
            
            // Register all hooks
            auto discordHook = std::unique_ptr<IHook>(new DiscordRPCHook());
            auto macHook = std::unique_ptr<IHook>(new MacAddressHook());
            
            hookManager.RegisterHook(std::move(discordHook));
            hookManager.RegisterHook(std::move(macHook));
            
            // Initialize all hooks
            bool result = hookManager.InitializeAllHooks();
            HookManager::DebugLog("[*] Hook initialization %s\n", result ? "successful" : "failed");
            return result;
        }
        case DLL_PROCESS_DETACH:
        {
            HookManager::DebugLog("[*] DLL_PROCESS_DETACH\n");
            HookManager::GetInstance().CleanupAllHooks();
            
            // Debug modunu kapat
            HookManager::EnableDebug(false);
            break;
        }
    }
    return TRUE;
} 