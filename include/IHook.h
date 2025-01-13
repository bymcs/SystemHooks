#pragma once

class IHook {
public:
    IHook() = default;
    virtual ~IHook() = default;
    virtual bool Initialize() = 0;
    virtual void Cleanup() = 0;
    virtual const char* GetName() const = 0;
}; 