#pragma once

#include <vector>
#include <memory>
#include "IHook.h"

class HookManager {
public:
    static HookManager& GetInstance();
    
    bool InitializeAllHooks();
    void CleanupAllHooks();
    void RegisterHook(std::unique_ptr<IHook> hook);
    
    // Debug functions
    static void EnableDebug(bool enable = true);
    static void InitDebugConsole();
    static void DebugLog(const char* format, ...);

private:
    HookManager() = default;
    ~HookManager() = default;
    HookManager(const HookManager&) = delete;
    HookManager& operator=(const HookManager&) = delete;

private:
    static HookManager* instance;
    std::vector<std::unique_ptr<IHook>> hooks;
    static FILE* debugConsole;
    static bool enableDebug;
}; 