#include <Windows.h>
#include <iostream>
#include "pch.h"

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

// Ativa um cheat com mensagem de confirmação
void ActivateCheat(const wchar_t* cheatCode, const wchar_t* cheatName) {
    SimulateKeyPress(cheatCode);
    MessageBox(NULL, cheatName, L"Cheat Ativado", MB_OK);
}

// Lógica principal do Mod Menu
void ActivateCheats() {
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            ActivateCheat(L"nosforeverever", L"Nitro Infinito");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            ActivateCheat(L"5grand5grand", L"Dinheiro Infinito");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F3) & 0x8000) {
            ActivateCheat(L"bigredfiredrive", L"Fire Truck Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F4) & 0x8000) {
            ActivateCheat(L"canyonalltheway", L"Todas as Pistas Desbloqueadas");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            ActivateCheat(L"chasingheadlocksappliedrashtalk", L"Mazda Dealership Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F6) & 0x8000) {
            ActivateCheat(L"chasingmobile", L"Corvette Z06 Interceptor Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F7) & 0x8000) {
            ActivateCheat(L"cooperlogofreeforyou", L"Cooper Tire Logo Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F8) & 0x8000) {
            ActivateCheat(L"friendlyheadlocksapplied", L"Speedbreaker Infinito");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            ActivateCheat(L"guesswhoisback", L"Cross Corvette Z06 Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F10) & 0x8000) {
            ActivateCheat(L"inthezoneskin", L"Autozone Vinyl Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F11) & 0x8000) {
            ActivateCheat(L"shinycarsarethebestcars", L"Chrome Cars Ativado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_F12) & 0x8000) {
            ActivateCheat(L"slowitdownwhenyouwant", L"Enhanced Brakes Ativado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_OEM_1) & 0x8000) { // Tecla ";"
            ActivateCheat(L"speed3foryou", L"Mazda Speed3 Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000) { // Tecla "="
            ActivateCheat(L"syntecskin", L"Castrol Syntec Vinyl Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) { // Tecla "-"
            ActivateCheat(L"trashtalking", L"Rhino (Quick Race) Desbloqueado");
            Sleep(500);
        }
        if (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000) { // Tecla ","
            ActivateCheat(L"watchoutthebigtruckishere", L"Dump Truck (Quick Race) Desbloqueado");
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
