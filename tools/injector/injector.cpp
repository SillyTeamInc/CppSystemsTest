//
// Created by emi on 10/12/26.
//
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdlib>
#include <cwchar>

static bool basename_matches(const wchar_t *path, const wchar_t *name, const wchar_t *withExt) {
    const wchar_t *base = wcsrchr(path, L'\\');
    base = base != nullptr ? base + 1 : path;
    return _wcsicmp(base, name) == 0 || _wcsicmp(base, withExt) == 0;
}

static bool image_path(DWORD pid, wchar_t *out, DWORD cap) {
    out[0] = L'\0';
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (h == nullptr) return false;
    DWORD size = cap;
    const bool ok = QueryFullProcessImageNameW(h, 0, out, &size) != 0;
    CloseHandle(h);
    return ok;
}

static DWORD find_pid(const wchar_t *name) {
    wchar_t withExt[MAX_PATH];
    swprintf(withExt, MAX_PATH, L"%ls.exe", name);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);
    DWORD pid = 0;
    DWORD unnamed = 0;
    int unnamedCount = 0;

    if (Process32FirstW(snap, &entry)) {
        do {
            wchar_t path[MAX_PATH];
            image_path(entry.th32ProcessID, path, MAX_PATH);

            if (basename_matches(entry.szExeFile, name, withExt) || basename_matches(path, name, withExt)) {
                pid = entry.th32ProcessID;
                break;
            }
            if (entry.szExeFile[0] == L'\0' && path[0] == L'\0') {
                unnamed = entry.th32ProcessID;
                unnamedCount++;
            }
        } while (Process32NextW(snap, &entry));
    }
    CloseHandle(snap);

    if (pid == 0) {
        // ????????????????????????????????????????????????????????? WHAT THE FUCKKKKK
        // This is Fucking Bullshit™
        // jokes on you, dumbass! the correct process is the blank one with no output from tlh. Fan Fucking tastic
        if (unnamedCount == 1) {
            wprintf(L"no name match, using the only unnamed process (%lu)\n", unnamed);
            pid = unnamed;
        } else {
            wprintf(L"no name match, unnamed process count: %d\n", unnamedCount);
        }
    }
    return pid;
}

int wmain(int argc, wchar_t **argv) {
    if (argc < 3) {
        wprintf(L"usage: injector <process name or pid> <dll path>");
        return 1;
    }

    wchar_t *end = nullptr;
    DWORD pid = wcstoul(argv[1], &end, 10);
    if (end == argv[1] || *end != L'\0') {
        pid = find_pid(argv[1]);
    }
    if (pid == 0) {
        wprintf(L"process not found: %ls", argv[1]);
        return 2;
    }

    wchar_t path[MAX_PATH];
    if (GetFullPathNameW(argv[2], MAX_PATH, path, nullptr) == 0) {
        wprintf(L"bad dll path\n");
        return 3;
    }

    HANDLE proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
                              PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (proc == nullptr) {
        wprintf(L"OpenProcess failed: %lu\n", GetLastError());
        return 4;
    }

    const SIZE_T bytes = (wcslen(path) + 1) * sizeof(wchar_t);
    void *remote = VirtualAllocEx(proc, nullptr, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (remote == nullptr || !WriteProcessMemory(proc, remote, path, bytes, nullptr)) {
        wprintf(L"remote write failed: %lu\n", GetLastError());
        CloseHandle(proc);
        return 5;
    }

    const auto loadLibrary = reinterpret_cast<LPTHREAD_START_ROUTINE>(
        GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW"));
    HANDLE thread = CreateRemoteThread(proc, nullptr, 0, loadLibrary, remote, 0, nullptr);
    if (thread == nullptr) {
        wprintf(L"CreateRemoteThread failed: %lu\n", GetLastError());
        CloseHandle(proc);
        return 6;
    }

    WaitForSingleObject(thread, INFINITE);
    DWORD result = 0;
    GetExitCodeThread(thread, &result);
    wprintf(result != 0 ? L"injected into pid %lu\n" : L"LoadLibrary returned null (pid %lu)\n", pid);

    VirtualFreeEx(proc, remote, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(proc);
    return result != 0 ? 0 : 7;
}
