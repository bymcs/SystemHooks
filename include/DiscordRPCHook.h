#pragma once

#include <Windows.h>
#include "IHook.h"

class DiscordRPCHook : public IHook {
public:
    static DiscordRPCHook& GetInstance();
    
    // IHook interface implementation
    bool Initialize() override;
    void Cleanup() override;
    const char* GetName() const override { return "DiscordRPCHook"; }
    
    // Make constructor and destructor public for unique_ptr
    DiscordRPCHook() = default;
    ~DiscordRPCHook() override = default;

private:
    DiscordRPCHook(const DiscordRPCHook&) = delete;
    DiscordRPCHook& operator=(const DiscordRPCHook&) = delete;

    static void WINAPI HookedDiscordUpdatePresence(const void* presence);
    static void* originalDiscordUpdatePresence;

private:
    static DiscordRPCHook* instance;
}; 