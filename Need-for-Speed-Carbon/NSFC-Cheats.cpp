#include <Windows.h>
#include <iostream>
#include <pch.h>

// Simula a entrada de um código de cheat
void SimulateKeyPress(const wchar_t* cheatCode) {
    for (int i = 0; cheatCode[i] != '\0'; i++) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0; // Virtual-Key Code não é usado aqui
        input.ki.wScan = cheatCode[i];
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input, sizeof(INPUT));

        // Liberar a tecla
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));

        Sleep(50); // Pausa para evitar envio rápido demais
    }
}

// Lógica principal do Mod Menu
void ActivateCheats() {
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            SimulateKeyPress(L"nosforeverever"); // Nitro infinito
            Sleep(500); // Evita múltiplas ativações rápidas
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            SimulateKeyPress(L"5grand5grand"); // Dinheiro infinito
            Sleep(500);
        }
        Sleep(100); // Previne uso excessivo de CPU
    }
}

// Função chamada ao injetar a DLL
DWORD WINAPI MainThread(LPVOID param) {
    MessageBox(NULL, L"DLL injetada com sucesso!", L"Mod Menu", MB_OK);
    ActivateCheats(); // Inicia a lógica do Mod Menu
    return 0;
}

// Função DllMain
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, MainThread, hModule, 0, 0); // Cria uma thread para o menu
    }
    return TRUE;
}
