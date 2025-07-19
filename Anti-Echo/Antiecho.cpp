#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <wininet.h>
#include <wincrypt.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <thread>
#include <locale>
#include <codecvt>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")

// Convert wstring to string (UTF-8)
std::string WStringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

// Known malicious file hashes (SHA-256)
const std::vector<std::string> maliciousHashes = {
    "d2fd03440efc603eb8e680db07e623d7da2be69a5f2ae0e9ef59493b34750c88"
};

// Suspicious domains
const std::vector<std::wstring> flaggedDomains = {
    L"api.echo.ac", L"echo.ac", L"echo-unprocessed-scans.s3.eu-west-2.amazonaws.com",
    L"d3mduebighmd0u.cloudfront.net", L"ip-ranges.amazonaws.com"
};

// Suspicious IPs
const std::vector<std::wstring> flaggedIPs = {
    L"104.16.123.96", L"104.16.124.96", L"104.26.6.44", L"104.26.7.44",
    L"108.156.91.129", L"108.156.91.83", L"108.156.91.89", L"108.156.91.90",
    L"142.250.73.131", L"142.250.73.67"
};

// Suspicious process names
const std::vector<std::wstring> suspiciousNames = {
    L"ntfsDump.exe",
    L"echo"
};

// Calculate SHA-256 hash of a file
std::string calculateFileHash(const std::wstring& filePath) {
    std::string ansiPath = WStringToString(filePath);

    BYTE hash[32];
    DWORD hashSize = sizeof(hash);
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = CreateFileA(ansiPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return "";

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) return "";
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) return "";

    BYTE buffer[4096];
    DWORD bytesRead = 0;
    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        CryptHashData(hHash, buffer, bytesRead, 0);
    }
    CloseHandle(hFile);

    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0)) return "";

    std::string result;
    char hex[3];
    for (DWORD i = 0; i < hashSize; ++i) {
        sprintf_s(hex, "%02x", hash[i]);
        result += hex;
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return result;
}

// Scan running processes
void scanRunningProcesses() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry = { sizeof(entry) };

    if (Process32FirstW(snapshot, &entry)) {
        do {
            std::wstring processName = entry.szExeFile;

            bool isSuspicious = false;
            for (const auto& pattern : suspiciousNames) {
                if (processName == pattern || processName.find(pattern) != std::wstring::npos) {
                    isSuspicious = true;
                    break;
                }
            }

            if (isSuspicious) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess) {
                    WCHAR widePath[MAX_PATH];
                    if (GetModuleFileNameExW(hProcess, NULL, widePath, MAX_PATH)) {
                        std::wstring pathW = widePath;
                        std::string hash = calculateFileHash(pathW);
                        for (const auto& knownHash : maliciousHashes) {
                            if (hash == knownHash) {
                                std::wcout << L"[DETECTED] Malicious file: " << pathW << std::endl;
                                TerminateProcess(hProcess, 0);
                                std::wcout << L"[ACTION] Terminated process: " << processName << std::endl;
                                break;
                            }
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
}

// Simulated DNS/IP connection check (placeholder)
void scanNetworkIndicators() {
    std::wcout << L"[*] Checking for known domains and IPs (simulated)...\n";
    for (const auto& domain : flaggedDomains) {
        std::wcout << L"[Domain Monitor] " << domain << std::endl;
    }
    for (const auto& ip : flaggedIPs) {
        std::wcout << L"[IP Monitor] " << ip << std::endl;
    }
}

int main() {
    system("cls");
    std::wcout << L"=============================================\n";
    std::wcout << L"     EchoAC & ntfsDump Detection Module      \n";
    std::wcout << L"     Game Hacking Fundamentals Project       \n";
    std::wcout << L"=============================================\n\n";

    while (true) {
        scanRunningProcesses();
        scanNetworkIndicators();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}
