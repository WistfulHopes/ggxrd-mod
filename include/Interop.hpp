#pragma once

#include "SdkHeaders.hpp"
#include "GameDefines.hpp"
#include <windows.h>
#include <chrono>
#include "rust/cxx.h"

bool AreGNamesValid()
{
    if (FName::Names()
        && !FName::Names()->empty()
        && (FName::Names()->capacity() > FName::Names()->size()))
    {
        return true;
    }

    return false;
}

bool AreGObjectsValid()
{
    if (UObject::GObjObjects()
        && !UObject::GObjObjects()->empty()
        && (UObject::GObjObjects()->capacity() > UObject::GObjObjects()->size()))
    {
        return true;
    }

    return false;
}

bool AreGlobalsValid()
{
    return (AreGObjectsValid() && AreGNamesValid());
}

bool find_globals()
{
    if (!UObject::GObjObjects() && !FName::Names())
    {
        // Populate the GObject and GName addresses, remember to replace "PlaceholderSDK" with your own.
        GObjects = reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + GObjects_Offset);
        GNames = reinterpret_cast<TArray<FNameEntry*>*>(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + GNames_Offset);
    }

    for (int i = 0; i < 10; i++)
    {
        if (AreGlobalsValid()) return true;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return false;
}

const FString& to_string(const rust::String& string)
{
    return FString(std::string(string));
}

rust::String from_string(const FString& string)
{
    return string.ToString();
}