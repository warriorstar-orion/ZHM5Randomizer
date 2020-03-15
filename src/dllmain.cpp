﻿#include <windows.h>
#include <Unknwnbase.h>

#include <filesystem>
#include <iostream>

#include "Config.h"
#include "Console.h"
#include "RandomisationMan.h"
#include "SceneLoadObserver.h"

typedef DWORD64(__stdcall *DIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID,
                                               LPVOID *, LPUNKNOWN);
DIRECTINPUT8CREATE fpDirectInput8Create;

extern "C" __declspec(dllexport) DWORD64
    __stdcall DirectInput8Create(HINSTANCE hinst, DWORD dwVersion,
                                 REFIID riidltf, LPVOID *ppvOut,
                                 LPUNKNOWN punkOuter) {
  return fpDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void loadOriginalDInput() {
  char syspath[320];
  HMODULE hMod;
  GetSystemDirectoryA(syspath, 320);
  strcat_s(syspath, "\\dinput8.dll");
  hMod = LoadLibraryA(syspath);
  if (hMod != NULL) {
    fpDirectInput8Create =
        (DIRECTINPUT8CREATE)GetProcAddress(hMod, "DirectInput8Create");
    printf("fpDirectInput8Create: 0x%I64x\n", (uintptr_t)fpDirectInput8Create);
  } else {
    MessageBoxA(NULL, "Failed to load DirectInput dll", "", NULL);
  }
}

std::unique_ptr<RandomisationMan> randomisation_man;
std::unique_ptr<SceneLoadObserver> scene_load_observer;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
      Config::base_directory =
          std::filesystem::current_path().generic_string();  //..\\HITMAN2
      Config::loadConfig();

      if (Config::showDebugConsole) Console::spawn();

      loadOriginalDInput();

      randomisation_man = std::make_unique<RandomisationMan>();
      scene_load_observer = std::make_unique<SceneLoadObserver>();

      auto loadConfigCallback = [](const SSceneInitParameters *sip) {
        Config::loadConfig();
      };
      scene_load_observer->registerSceneLoadCallback(loadConfigCallback);

      auto loadCallback =
          std::bind(&RandomisationMan::initializeRandomizers,
                    randomisation_man.get(), std::placeholders::_1);
      scene_load_observer->registerSceneLoadCallback(loadCallback);
    } break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}
