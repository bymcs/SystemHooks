#include "../include/DiscordRPCHook.h"
#include <detours.h>

// Static member initialization
DiscordRPCHook* DiscordRPCHook::instance = nullptr;
void* DiscordRPCHook::originalDiscordUpdatePresence = nullptr;

// Function pointer type for Discord_UpdatePresence
typedef void (WINAPI* fpDiscordUpdatePresence)(const void* presence);

DiscordRPCHook& DiscordRPCHook::GetInstance() {
    static DiscordRPCHook instance;
    return instance;
}

bool DiscordRPCHook::Initialize() {
    // Discord RPC DLL'ini yükle
    HMODULE hDiscordRPC = GetModuleHandleA("discord-rpc.dll");
    if (!hDiscordRPC) {
        return false;
    }

    // Discord_UpdatePresence fonksiyonunun adresini al (VA: 0x2910)
    originalDiscordUpdatePresence = GetProcAddress(hDiscordRPC, "Discord_UpdatePresence");
    if (!originalDiscordUpdatePresence) {
        return false;
    }

    // Hook işlemini başlat
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    
    LONG error = DetourAttach(&(PVOID&)originalDiscordUpdatePresence, HookedDiscordUpdatePresence);
    if (error != NO_ERROR) {
        DetourTransactionAbort();
        return false;
    }
    
    return DetourTransactionCommit() == NO_ERROR;
}

void DiscordRPCHook::Cleanup() {
    if (originalDiscordUpdatePresence) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)originalDiscordUpdatePresence, HookedDiscordUpdatePresence);
        DetourTransactionCommit();
    }
}

void WINAPI DiscordRPCHook::HookedDiscordUpdatePresence(const void* presence) {
    // Burada presence yapısını manipüle edebilirsiniz
    // Örnek olarak, presence'i NULL yaparak Discord durumunu temizleyebilirsiniz
    // veya presence yapısını değiştirerek farklı bir durum gösterebilirsiniz
    
    // Orijinal fonksiyonu çağır (isteğe bağlı)
    // ((fpDiscordUpdatePresence)originalDiscordUpdatePresence)(presence);
    
    // Veya hiçbir şey yapma (Discord durumunu güncellemeyi engelle)
    return;
} 