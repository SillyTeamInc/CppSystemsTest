set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)
set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_CXX_COMPILER clang-cl)
set(CMAKE_LINKER lld-link)
set(CMAKE_RC_COMPILER llvm-rc)
set(CMAKE_AR llvm-lib)
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

set(XWIN $ENV{HOME}/.xwin)
#[[set(CMAKE_C_FLAGS_INIT   "--target=x86_64-pc-windows-msvc -fuse-ld=lld")
set(CMAKE_CXX_FLAGS_INIT "--target=x86_64-pc-windows-msvc -fuse-ld=lld")]]
set(CMAKE_C_COMPILER_TARGET x86_64-pc-windows-msvc)
set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc)

foreach(d crt/include sdk/include/ucrt sdk/include/um sdk/include/shared sdk/include/winrt sdk/include/cppwinrt)
    string(APPEND CMAKE_C_FLAGS_INIT   " /imsvc ${XWIN}/${d}")
    string(APPEND CMAKE_CXX_FLAGS_INIT " /imsvc ${XWIN}/${d}")
endforeach()

foreach(l crt/lib/x86_64 sdk/lib/um/x86_64 sdk/lib/ucrt/x86_64)
    string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT    " /libpath:${XWIN}/${l}")
    string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " /libpath:${XWIN}/${l}")
endforeach()