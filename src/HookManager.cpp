#include "../include/HookManager.h"
#include <Windows.h>
#include <string>
#include <cstdio>
#include <cstdarg>

HookManager* HookManager::instance = nullptr;
FILE* HookManager::debugConsole = nullptr;
bool HookManager::enableDebug = false;

void HookManager::EnableDebug(bool enable) {
    enableDebug = enable;
    if (enable) {
        InitDebugConsole();
    } else if (debugConsole) {
        fclose(debugConsole);
        FreeConsole();
        debugConsole = nullptr;
    }
}

void HookManager::InitDebugConsole() {
    if (!enableDebug || debugConsole) return;
    
    if (AllocConsole()) {
        freopen_s(&debugConsole, "CONOUT$", "w", stdout);
        SetConsoleTitleA("SystemHooks Debug Console");
        
        // Konsol penceresini ayarla
        HWND consoleWindow = GetConsoleWindow();
        SetWindowPos(consoleWindow, 0, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
        
        // Başlangıç mesajı
        DebugLog("SystemHooks Debug Console\n");
        DebugLog("------------------------\n");
    }
}

void HookManager::DebugLog(const char* format, ...) {
    if (!enableDebug || !debugConsole) return;
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

HookManager& HookManager::GetInstance() {
    if (!instance) {
        instance = new HookManager();
    }
    return *instance;
}

bool HookManager::InitializeAllHooks() {
    bool success = true;
    for (const auto& hook : hooks) {
        DebugLog("[*] Initializing hook: %s\n", hook->GetName());
        if (!hook->Initialize()) {
            DebugLog("[!] Failed to initialize hook: %s\n", hook->GetName());
            success = false;
        } else {
            DebugLog("[+] Successfully initialized hook: %s\n", hook->GetName());
        }
    }
    return success;
}

void HookManager::CleanupAllHooks() {
    for (const auto& hook : hooks) {
        DebugLog("[*] Cleaning up hook: %s\n", hook->GetName());
        hook->Cleanup();
    }
    hooks.clear();
    
    if (debugConsole) {
        fclose(debugConsole);
        FreeConsole();
        debugConsole = nullptr;
    }
}

void HookManager::RegisterHook(std::unique_ptr<IHook> hook) {
    DebugLog("[*] Registering hook: %s\n", hook->GetName());
    hooks.push_back(std::move(hook));
} 