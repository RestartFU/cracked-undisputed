// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "hook.hpp"
#include <string>
typedef void*(__stdcall* tstrcat) (void* Src, unsigned __int64 a2, __int64 a3, __int64 a4, char* Srca, size_t Size);
tstrcat o_strcat2 = (tstrcat)((uintptr_t)GetModuleHandleA(NULL) + (uintptr_t)0x730E0);
const char* localhost = "127.0.0.1:1337/";

void* __fastcall strcathook(void* Src, unsigned __int64 a2, __int64 a3, __int64 a4, char* Srca, size_t Size) 
{
    strcpy(Srca, localhost);
    Size = strlen(localhost);

    hook::disable(o_strcat2);
    void* ret = o_strcat2(Src, a2, a3, a4, Srca, Size);
    hook::enable(o_strcat2);
    
    return ret;
}


unsigned char timerSkip_code[1] = { 0xEB };
unsigned char NOPS[13] = { 0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90 };

void start()
{
    hook::make_hook(o_strcat2, &strcathook, true);

    while (true)
    {
        Sleep(1);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, NULL, NULL, NULL);
    }
    return TRUE;
}
