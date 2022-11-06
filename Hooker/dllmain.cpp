// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"

// function to hook
bool hook(void* addrToHook, void* function, int hookLen)
{
    if (hookLen < 5) return false; // impossible to hook

    DWORD oldProtection;
    VirtualProtect(addrToHook, hookLen, PAGE_EXECUTE_READWRITE, &oldProtection);

    memset(addrToHook, '\x90', hookLen); // replacing old ASM with NOPs

    DWORD relAddr = ((DWORD)function - (DWORD)addrToHook) - 5; // function address - address to hook - JMP opcode size

    *(BYTE*)addrToHook = 0xE9; // JMP ...
    *(DWORD*)((DWORD)addrToHook + 1) = relAddr; // JMP 0xDEADBEEF

    DWORD temp; // useless variable to store PAGE_EXECUTE_READWRITE
    VirtualProtect(addrToHook, hookLen, oldProtection, &temp);

    return true;
}

// "new" code for IsDebuggerPresent
void __declspec(naked) idgbpHook()
{
    __asm 
    {
        mov eax, [30]
        mov eax, 0
        ret
    }
}

// hacking thread
BOOL WINAPI thread(HMODULE hmod)
{
    bool running = true;

    /* Original code for the function (copied from x32dbg)
        7591C8F0 | 64:A1 30000000           | mov eax,dword ptr fs:[30]               |
        7591C8F6 | 0FB640 02                | movzx eax,byte ptr ds:[eax+2]           |
        7591C8FA | C3                       | ret                                     |
    */

    DWORD hookAddr = 0x7591C8FB; // might change with your program
    int length = 11;
    DWORD jumpBack = hookAddr - length;


    hook((void*)hookAddr, idgbpHook, length);

    while (running)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            running = false;

        Sleep(50);
    }

    FreeLibraryAndExitThread(hmod, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)thread, hModule, 0, nullptr); // DLL start
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

