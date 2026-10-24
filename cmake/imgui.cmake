set(IMGUI_VERSION "v1.93.0" CACHE STRING "ImGui version")
option(IMGUI_BUILD_DEMO "Compile imgui_demo.cpp" ON)
option(IMGUI_BUILD_STDLIB "Compile misc/cpp/imgui_stdlib.cpp" OFF)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG ${IMGUI_VERSION}
        GIT_SHALLOW TRUE
        DOWNLOAD_ONLY YES
)

if (imgui_ADDED AND NOT TARGET imgui)
    set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    )

    if (IMGUI_BUILD_DEMO)
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/imgui_demo.cpp)
    endif ()

    if (IMGUI_BUILD_STDLIB)
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp)
    endif ()

    add_library(imgui STATIC ${IMGUI_SOURCES})
    add_library(imgui::imgui ALIAS imgui)

    target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
    )
    if (IMGUI_BUILD_STDLIB)
        target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/misc/cpp)
    endif ()

    target_compile_features(imgui PUBLIC cxx_std_17)
    target_link_libraries(imgui PUBLIC d3d11 dxgi d3dcompiler dwmapi imm32)

    set_target_properties(imgui PROPERTIES
            UNITY_BUILD FALSE
            POSITION_INDEPENDENT_CODE ON
            MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDLL>$<$<CONFIG:Release>:MultiThreaded>"
    )
endif ()