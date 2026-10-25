//
// Created by emi on 10/25/26.
//
#include "exceptions.hpp"
#include <windows.h>

LONG WINAPI TlExceptionHandler(const PEXCEPTION_POINTERS pExceptionInfo) {
    spdlog::error("Unhandled exception occurred: code=0x{:X}, address=0x{:p}", pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);
    int a = MessageBoxA(nullptr, "Something has fucked up, attach debugger and click RETRY!!!",
                        "Unhandled Exception", MB_ABORTRETRYIGNORE | MB_ICONERROR);
    if (a == IDABORT) {
        spdlog::info("aborting process.");
        ExitProcess(1);
    } else if (a == IDRETRY) {
        return EXCEPTION_CONTINUE_EXECUTION;
    } else {
        return EXCEPTION_EXECUTE_HANDLER;
    }
}


void util::exceptions::attach_handler() {
    SetUnhandledExceptionFilter(TlExceptionHandler);
    spdlog::info("attached unhandled exception filter");
}

void util::exceptions::remove_handler() {
    SetUnhandledExceptionFilter(nullptr);
    spdlog::info("removed unhandled exception filter");
}

void util::exceptions::throw_error(const std::string& message) {
    spdlog::error(message);
    throw std::runtime_error(message);
}
