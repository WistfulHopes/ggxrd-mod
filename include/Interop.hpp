#pragma once

#include "SdkHeaders.hpp"
#include "GameDefines.hpp"
#include <windows.h>
#include <chrono>
#include "rust/cxx.h"

static constexpr int32_t INSTANCES_INTERATE_OFFSET = 10;

namespace library_private {    
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
}

using namespace library_private;

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

template<typename T> T* GetInstanceOf()
{
    if (std::is_base_of<UObject, T>::value && UObject::GObjObjects())
    {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--)
        {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>())
            {
                if (uObject->GetFullName().find("Default__") == std::string::npos)
                {
                    return static_cast<T*>(uObject);
                }
            }
        }
    }

    return nullptr;
}

const FString& to_string(const rust::String& string)
{
    return FString(std::string(string));
}

rust::String from_string(const FString& string)
{
    return string.ToString();
}

int change_scene(int scene_id)
{
    EUE_SCENE_ID scene;

    try
    {
        if (scene_id > 30)
        {
            throw std::out_of_range("");
        }
        scene = static_cast<EUE_SCENE_ID>(scene_id);
    }
    catch (std::invalid_argument)
    {
        return 1;
    }
    catch (std::out_of_range)
    {
        return 2;
    }

    auto GameCommon = GetInstanceOf<UREDGameCommon>();
    if (!GameCommon)
    {
        return 3;
    }

    auto GameInfo = GetInstanceOf<AREDGameInfo>();
    if (!GameInfo)
    {
        return 4;
    }

    GameInfo->ConsoleCommand(GameCommon->GetNextSceneCommand(scene, false), false);
}