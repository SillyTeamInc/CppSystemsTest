cmake_minimum_required(VERSION 4.3)
project(CppSystemsTest)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "this mod only supports x64 builds")
endif ()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


set(CPM_DOWNLOAD_VERSION 0.40.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
    )
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.1
        OPTIONS
        "SPDLOG_BUILD_EXAMPLE OFF"
        "SPDLOG_BUILD_TESTS OFF"
)

CPMAddPackage(
        NAME minhook
        GITHUB_REPOSITORY TsudaKageyu/minhook
        VERSION 1.3.4
)

CPMAddPackage(
        NAME pfr
        GITHUB_REPOSITORY boostorg/pfr
        GIT_TAG boost-1.92.0
        OPTIONS "BUILD_TESTING OFF"
)

CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        VERSION 0.9.5
)

CPMAddPackage(
        NAME libhat
        GITHUB_REPOSITORY BasedInc/libhat
        GIT_TAG v0.11.1
        OPTIONS "LIBHAT_TESTING OFF" "LIBHAT_EXAMPLES OFF" "LIBHAT_INSTALL_TARGET OFF"
)

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 1.0.1
)

CPMAddPackage(
        NAME kiero
        GITHUB_REPOSITORY BasedInc/kiero
        GIT_TAG master
        OPTIONS "KIERO_INCLUDE_D3D11 ON" "KIERO_INCLUDE_D3D12 ON" "KIERO_BUILD_EXAMPLES OFF"
)

set_property(TARGET libhat kiero spdlog minhook PROPERTY UNITY_BUILD FALSE)

if(MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /clang:-g /clang:-gdwarf-4 /JMC /bigobj /Zc:inline /EHsc /permissive-)
        add_link_options(/DEBUG:FULL /INCREMENTAL /DEBUG:DWARF)
    else()
        add_compile_options(/O2 /GL /bigobj /EHsc /permissive-)
        add_link_options(/LTCG)
    endif()
endif()

# am i stupid..?
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
add_compile_definitions(__MODE_LIBRARY)
add_library(CppSystemsTest SHARED src/main.cpp
        src/events/keyboard_event.hpp
        tools/injector/injector.cpp)

target_sources(CppSystemsTest PRIVATE
        src/pch.hpp

        src/core/core.cpp
        src/core/core.hpp

        src/hvs/hvs.hpp

        src/system/core_system.hpp
        src/system/event_system.hpp
        src/system/complex_system.hpp
        src/system/generic_system.hpp
        src/system/systems_manager.cpp
        src/system/systems_manager.hpp

        src/events/moused_event.hpp

        src/system/features/feature_system.hpp
        src/system/features/module_system.hpp

        src/system/hooks/base/detour.cpp
        src/system/hooks/base/detour.hpp
        src/system/hooks/base/hook.hpp
        src/system/hooks/base/hook_system.cpp
        src/system/hooks/base/hook_system.hpp
        src/system/hooks/base/hook_base.cpp
        src/system/hooks/base/hook_base.hpp

        src/system/hooks/mouse_hook.cpp
        src/system/hooks/mouse_hook.hpp

        src/system/signatures/basic_signature_system.hpp
        src/system/signatures/game_signatures.hpp
        src/system/signatures/sig.hpp

        src/util/exceptions.hpp
        src/util/memory.cpp
        src/util/memory.hpp
)

target_compile_options(CppSystemsTest PRIVATE -Wno-c++26-extensions)
target_link_options(CppSystemsTest PRIVATE /DYNAMICBASE:NO /BASE:0x180000000)
set_target_properties(CppSystemsTest PROPERTIES CXX_STANDARD 26)

target_include_directories(CppSystemsTest PRIVATE src)

target_link_libraries(CppSystemsTest PRIVATE glm spdlog::spdlog kiero magic_enum libhat::libhat Boost::pfr minhook runtimeobject.lib)

set_property(TARGET CppSystemsTest spdlog magic_enum glm libhat minhook PROPERTY MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDLL>$<$<CONFIG:Release>:MultiThreaded>")

target_precompile_headers(CppSystemsTest PRIVATE "src/pch.hpp")

add_subdirectory(tools/injector)
add_dependencies(CppSystemsTest injector)