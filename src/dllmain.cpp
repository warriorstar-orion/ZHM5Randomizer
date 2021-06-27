#include "ClientValidation.h"
#include "Config.h"
#include "Console.h"
#include "RandomisationMan.h"
#include "SceneLoadObserver.h"
#include <Unknwnbase.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <windows.h>

#ifdef TEST_INTERFACE
#include "TestInterface.h"
#endif


typedef DWORD64(__stdcall* DIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DIRECTINPUT8CREATE fpDirectInput8Create;

extern "C" __declspec(dllexport) DWORD64
__stdcall DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
    return fpDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void loadOriginalDInput() {
    char syspath[320];
    HMODULE hMod;
    GetSystemDirectoryA(syspath, 320);
    strcat_s(syspath, "\\dinput8.dll");
    hMod = LoadLibraryA(syspath);
    if(hMod != NULL) {
        fpDirectInput8Create = (DIRECTINPUT8CREATE)GetProcAddress(hMod, "DirectInput8Create");
        printf("fpDirectInput8Create: 0x%I64x\n", (uintptr_t)fpDirectInput8Create);
    } else {
        MessageBoxA(NULL, "Failed to load DirectInput dll", "", NULL);
    }
}

// Checks if the current client is legitimate.
bool checkClient() {
    const char* sigErrorMsgFmt = "Signature validation failed with error: {}!\n\nPlease verify the "
                                 "integrity of your game files and make "
                                 "sure you are using a legitimate client copy.";
    std::string errorMsg;

    auto clientValidationStatus = Validation::isValidClient();
    switch(clientValidationStatus) {
    case Validation::Status::Ok:
        break;
    case Validation::Status::NotTrusted:
        errorMsg = std::format(sigErrorMsgFmt, "NotTrusted");
        break;
    case Validation::Status::InvalidSubjectName:
        errorMsg = std::format(sigErrorMsgFmt, "InvalidSubjectName");
        break;
    case Validation::Status::UnkownError:
    default:
        errorMsg = std::format(sigErrorMsgFmt, "UnkownError");
    }

    if(clientValidationStatus != Validation::Status::Ok) {
        MessageBoxA(NULL, errorMsg.c_str(), "Signature validation error", NULL);
        return false;
    }

    return true;
}

std::unique_ptr<RandomisationMan> randomisation_man;
std::unique_ptr<SceneLoadObserver> scene_load_observer;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        loadOriginalDInput();

        if(!checkClient())
            exit(0);

        Config::loadConfig();
        if(Config::showDebugConsole)
            Console::spawn();

        randomisation_man = std::make_unique<RandomisationMan>();

        auto loadConfigCallback = [](const SSceneInitParameters* sip) { Config::loadConfig(); };
        auto loadCallback = std::bind(&RandomisationMan::initializeRandomizers,
                                      randomisation_man.get(), std::placeholders::_1);

        scene_load_observer = std::make_unique<SceneLoadObserver>();
        scene_load_observer->registerSceneLoadCallback(loadConfigCallback);
        scene_load_observer->registerSceneLoadCallback(loadCallback);

#ifdef TEST_INTERFACE
        TestInterface::run();
#endif
    } break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
