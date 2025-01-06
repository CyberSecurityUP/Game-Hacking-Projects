/*
Cheat feito por Joas A Santos
*/
#include "pch.h"

// Função para calcular o endereço dinâmico
DWORD_PTR ResolvePointer(HANDLE hProcess, DWORD_PTR baseAddress, const std::vector<DWORD>& offsets) {
    DWORD_PTR addr = baseAddress;

    for (size_t i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProcess, (LPCVOID)addr, &addr, sizeof(addr), NULL); // Lê o valor no endereço atual
        addr += offsets[i]; // Adiciona o offset
    }

    return addr;
}

// Função para alterar o valor do dinheiro
void SetMoney(HANDLE hProcess, DWORD_PTR address, int value) {
    WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(value), NULL); // Escreve o novo valor no endereço
}

// Função principal para monitorar a tecla F1
void MonitorKeyPress(HANDLE hProcess) {
    // Base Address e offsets coletados
    DWORD_PTR baseAddress = (DWORD_PTR)GetModuleHandle(L"NFSC.exe") + 0x0068AD08; // Base address do Cheat Engine
    std::vector<DWORD> offsets = { 0x4, 0x54 }; // Offsets extraídos do Cheat Engine

    while (true) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            DWORD_PTR moneyAddress = ResolvePointer(hProcess, baseAddress, offsets); // Resolve o ponteiro dinâmico
            SetMoney(hProcess, moneyAddress, 9999999); // Altera o valor do dinheiro para 9.999.999
            MessageBox(NULL, L"Dinheiro alterado para 9.999.999", L"Cheat Ativado", MB_OK);
            Sleep(500); 
        }
        Sleep(100); 
    }
}

// Thread principal
DWORD WINAPI MainThread(LPVOID param) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (!hProcess) {
        MessageBox(NULL, L"Falha ao abrir o processo!", L"Erro", MB_OK);
        return 1;
    }

    MessageBox(NULL, L"DLL injetada com sucesso", L"Infinity Money Cheat DLL", MB_OK);
    MonitorKeyPress(hProcess); // Inicia o monitoramento de teclas
    CloseHandle(hProcess); 
    return 0;
}

// Função DllMain
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, MainThread, hModule, 0, 0); // Cria uma thread para executar o cheat
    }
    return TRUE;
}
