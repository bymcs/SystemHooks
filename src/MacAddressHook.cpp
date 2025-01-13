#include "../include/MacAddressHook.h"
#include <detours.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "detours.lib")

// Static member initialization
MacAddressHook* MacAddressHook::instance = nullptr;
std::random_device MacAddressHook::rd;
std::mt19937 MacAddressHook::gen(rd());
std::uniform_int_distribution<> MacAddressHook::dis(0, 255);

// Original function pointer
static fpGetAdaptersInfo originalGetAdaptersInfo = GetAdaptersInfo;

MacAddressHook& MacAddressHook::GetInstance() {
    static MacAddressHook instance;
    return instance;
}

bool MacAddressHook::Initialize() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    LONG error = DetourAttach(&(PVOID&)originalGetAdaptersInfo, HookedGetAdaptersInfo);
    if (error != NO_ERROR) {
        DetourTransactionAbort();
        return false;
    }
    return DetourTransactionCommit() == NO_ERROR;
}

void MacAddressHook::Cleanup() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)originalGetAdaptersInfo, HookedGetAdaptersInfo);
    DetourTransactionCommit();
}

void MacAddressHook::GenerateRandomMac(BYTE* macAddress) {
    // İlk byte'ın son biti 1 olmamalı (unicast)
    // İlk byte'ın ikinci son biti 0 olmalı (globally unique)
    macAddress[0] = (dis(gen) & 0xFC) | 0x02;  // Locally administered, unicast
    
    // Diğer 5 byte rastgele
    for (int i = 1; i < 6; i++) {
        macAddress[i] = dis(gen);
    }
}

DWORD WINAPI MacAddressHook::HookedGetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen) {
    // Call the original function using the saved pointer
    DWORD result = originalGetAdaptersInfo(pAdapterInfo, pOutBufLen);

    // If successful and adapter info is available
    if (result == NO_ERROR && pAdapterInfo != NULL) {
        PIP_ADAPTER_INFO adapter = pAdapterInfo;
        while (adapter) {
            GenerateRandomMac(adapter->Address);
            adapter = adapter->Next;
        }
    }

    return result;
} 