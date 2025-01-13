#pragma once

#include <Windows.h>
#include <iphlpapi.h>
#include <iptypes.h>
#include <random>
#include "IHook.h"

// Function pointer type definition
typedef DWORD(WINAPI* fpGetAdaptersInfo)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);

class MacAddressHook : public IHook {
public:
    static MacAddressHook& GetInstance();
    
    // IHook interface implementation
    bool Initialize() override;
    void Cleanup() override;
    const char* GetName() const override { return "MacAddressHook"; }
    
    // Make constructor and destructor public for unique_ptr
    MacAddressHook() = default;
    ~MacAddressHook() override = default;

private:
    MacAddressHook(const MacAddressHook&) = delete;
    MacAddressHook& operator=(const MacAddressHook&) = delete;

    static void GenerateRandomMac(BYTE* macAddress);
    static DWORD WINAPI HookedGetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);

private:
    static MacAddressHook* instance;
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<> dis;
}; 