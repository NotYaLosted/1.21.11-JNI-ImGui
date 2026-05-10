#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>

#include "utils/java/java.h"
#include "utils/class/classloader.h"
#include "sdk/minecraft/minecraft.h"
#include "sdk/minecraft/world.h"
#include "sdk/minecraft/mappings/mappings.h"
#include "utils/hooks/Hook.h"
#include "utils/gui/gui.h"


static bool is_unloading = false;

DWORD WINAPI thread_entry(LPVOID param) {
    HMODULE hmodule = (HMODULE)param;

    DisableThreadLibraryCalls(hmodule);


    FreeConsole();
    if (!AllocConsole()) {
        if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
            MessageBoxA(NULL, "Failed to allocate or attach console", "Error", MB_ICONERROR);
            FreeLibraryAndExitThread(hmodule, 0);
            return 1;
        }
    }

    FILE* f;
    if (freopen_s(&f, "CONOUT$", "w", stdout) != 0) {
        MessageBoxA(NULL, "Failed to redirect stdout", "Error", MB_ICONERROR);
        FreeConsole();
        FreeLibraryAndExitThread(hmodule, 0);
        return 1;
    }

    SetConsoleTitle("jni");
    std::cout << "[+] Initializing hooks...\n";

    if (Hook::init()) {
        std::cout << "[-] Hook init failed\n";
        MessageBoxA(NULL, "Hook init failed", "Error", MB_ICONERROR);
        Sleep(5000);
        Hook::shutdown();
        fclose(f);
        FreeConsole();
        FreeLibraryAndExitThread(hmodule, 0);
        return 1;
    }

    std::cout << "[+] Hooks initialized\n";

    if (!g_java.init()) {
        std::cout << "[-] JNI init failed\n";
        MessageBoxA(NULL, "JNI init failed", "Error", MB_ICONERROR);
        Hook::shutdown();
        fclose(f);
        FreeConsole();
        FreeLibraryAndExitThread(hmodule, 0);
        return 1;
    }

    std::cout << "[+] JNI initialized\n";

    if (!sdk::classloader::init(g_java.env)) {
        std::cout << "[-] Classloader init failed\n";
        MessageBoxA(NULL, "Classloader init failed", "Error", MB_ICONERROR);
        Hook::shutdown();
        g_java.detach();
        fclose(f);
        FreeConsole();
        FreeLibraryAndExitThread(hmodule, 0);
        return 1;
    }

    std::cout << "[+] Classloader: "
        << (sdk::classloader::is_fabric() ? "Fabric" : "Vanilla/Forge")
        << "\n";

    bool was_in_game = false;


    while (!GUI::get_unload() && !is_unloading) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        auto mc = c_minecraft::get();
        if (!mc || !mc->is_valid()) {
            if (was_in_game) {
                std::cout << "[!] Lost connection to Minecraft\n";
                was_in_game = false;
                c_minecraft::reset();
            }
            continue;
        }

        auto lp = mc->get_local_player();
        if (!lp || !lp->is_valid()) {
            if (was_in_game) {
                std::cout << "[!] Player disconnected\n";
                was_in_game = false;
                c_minecraft::reset();
            }
            continue;
        }

        if (!was_in_game) {
            std::cout << "[+] Connected to Minecraft!\n";
            was_in_game = true;
        }
    }

    std::cout << "[+] Unload signal received, shutting down...\n";
    is_unloading = true;

 
    std::cout << "[-] Cleaning up GUI...\n";
    GUI::shutdown();

    std::cout << "[-] Shutting down hooks...\n";
    Hook::shutdown();

    std::cout << "[-] Cleaning up classloader...\n";
    sdk::classloader::cleanup(g_java.env);

    std::cout << "[-] Detaching JNI...\n";
    g_java.detach();


    c_minecraft::reset();

    std::cout << "[+] Cleanup complete! Unloading DLL...\n";


    Sleep(100);

    fclose(f);
    FreeConsole();

    FreeLibraryAndExitThread(hmodule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {

        DisableThreadLibraryCalls(hModule);

        HANDLE thread = CreateThread(NULL, 0, thread_entry, hModule, 0, NULL);
        if (thread) {
            CloseHandle(thread);
        }
        else {
            MessageBoxA(NULL, "Failed to create main thread", "Error", MB_ICONERROR);
        }
        break;
    }
    case DLL_PROCESS_DETACH: {

        if (!is_unloading) {
            is_unloading = true;

            GUI::shutdown();
            Hook::shutdown();

            if (g_java.env) {
                sdk::classloader::cleanup(g_java.env);
                g_java.detach();
            }
        }
        break;
    }
    }
    return TRUE;
}