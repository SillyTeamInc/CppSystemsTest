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

if(MSVC)
    add_compile_options(/EHsc)
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
        OPTIONS "KIERO_BUILD_MINHOOK OFF" "KIERO_INCLUDE_D3D11 ON" "KIERO_INCLUDE_D3D12 ON" "KIERO_BUILD_EXAMPLES OFF"
)

CPMAddPackage(
        NAME gsl
        GITHUB_REPOSITORY microsoft/GSL
        GIT_TAG v5.0.0
)

CPMAddPackage(
        NAME EnTT
        GITHUB_REPOSITORY skypjack/entt
        GIT_TAG fe8d7d78c4823e8a66a050bf86f5c6318cf76ce7
)

CPMAddPackage(
        NAME nlohmann_json
        GITHUB_REPOSITORY nlohmann/json
        VERSION 3.11.3
)

include(cmake/imgui.cmake)
set_property(TARGET libhat imgui kiero EnTT spdlog minhook PROPERTY UNITY_BUILD FALSE)

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
        tools/injector/injector.cpp
        src/sdk/mc/deps/application/gamecore/Platform_GameCore.hpp
        src/sdk/mc/deps/application/gamecore/WinMain.hpp
        src/core/sdk_base.hpp
        src/sdk/mc/deps/application/gamecore/AppPlatform_GameCore.hpp
        src/sdk/mc/client/game/MinecraftGame.hpp
        src/sdk/mc/deps/application/gamecore/GameHolder.hpp
        src/sdk/mc/client/game/ClientInstance.hpp
        src/core/core.cpp
        src/core/core.hpp
        src/events/module_state_change_event.hpp
        src/system/features/modules/module.cpp
        src/system/features/modules/module.hpp
        src/util/hash.hpp
        src/system/features/modules/module_system.cpp
        src/system/features/modules/misc/test_module.cpp
        src/system/features/modules/misc/test_module.hpp
        src/events/mod_tick_event.hpp
        src/util/types.hpp
        src/sdk/mc/deps/ecs/EntityId.hpp
        src/sdk/mc/deps/core/utility/optional_ref.hpp
        src/sdk/mc/world/actor/Actor.cpp
        src/sdk/mc/world/actor/Actor.hpp
        src/sdk/mc/entity/components/MoveInputComponent.hpp
        src/sdk/mc/platform/brstd/bitset.hpp
        src/sdk/mc/platform/brstd/detail/bitset_base.hpp
        src/sdk/mc/world/actor/ActorType.hpp
        src/sdk/mc/input/NewInteractionModel.hpp
        src/sdk/mc/server/commands/CommandPermissionLevel.hpp
        src/system/features/modules/misc/sprint.cpp
        src/system/features/modules/misc/sprint.hpp
        src/system/hooks/mob_base_tick_hook.cpp
        src/system/hooks/mob_base_tick_hook.hpp
        src/events/base_tick_event.hpp
        src/sdk/mc/world/actor/ActorUniqueID.hpp
        src/sdk/mc/options/option_types/Option.hpp
        src/sdk/mc/options/option_types/OptionType.hpp
        src/sdk/mc/options/option_types/OptionOwnerType.hpp
        src/sdk/mc/options/option_types/OptionID.hpp
        src/sdk/mc/options/option_types/OptionResetFlags.hpp
        src/sdk/mc/options/option_types/IntOption.hpp
        src/sdk/mc/options/option_types/FloatOption.hpp
        src/sdk/mc/options/IOptionsReader.hpp
        src/util/option_dumper/option_dumper.cpp
        src/util/option_dumper/option_dumper.hpp
        src/system/features/modules/misc/fullbright.cpp
        src/system/features/modules/misc/fullbright.hpp
        src/sdk/mc/options/option_types/BoolOption.hpp
        src/sdk/mc/options/option_types/EnumOption.hpp
        src/system/features/modules/misc/freelook.cpp
        src/system/features/modules/misc/freelook.hpp
        src/sdk/mc/entity/components/ActorHeadRotationComponent.hpp
        src/sdk/mc/entity/components/CameraComponent.hpp
        src/sdk/mc/deps/core/string/HashedString.hpp
        src/sdk/mc/entity/components/CameraDirectLookComponent.hpp
        src/sdk/mc/entity/components/CameraOrbitComponent.hpp
        src/sdk/mc/deps/minecraft_camera/Spherical.hpp
        src/sdk/mc/deps/minecraft_camera/CameraClientInstanceComponent.hpp
        src/sdk/mc/deps/minecraft_camera/ICameraClientInstance.hpp
        src/sdk/mc/deps/vanilla_camera/VanillaCameraApiComponent.hpp
        src/sdk/mc/deps/vanilla_camera/IVanillaCameraAPI.hpp
        src/sdk/mc/deps/minecraft_camera/CameraRegistry.hpp
        src/sdk/mc/deps/game_refs/OwnerPtr.hpp
        src/sdk/mc/deps/core/utility/NonOwnerPointer.hpp
        src/sdk/mc/deps/core/utility/EnableNonOwnerReferences.hpp
        src/system/render/render_system.cpp
        src/system/render/render_system.hpp
        src/system/hooks/render_hooks/present_hook.cpp
        src/system/hooks/render_hooks/present_hook.hpp
        src/system/hooks/render_hooks/execute_command_lists_hook.cpp
        src/system/hooks/render_hooks/execute_command_lists_hook.hpp
        src/system/hooks/render_hooks/resize_buffers_hook.cpp
        src/system/hooks/render_hooks/resize_buffers_hook.hpp
        src/events/render_event.hpp
        src/util/keyboard.cpp
        src/util/keyboard.hpp
        src/util/fonts.cpp
        src/util/fonts.hpp
        src/sdk/mc/entity/components/FlagComponent.hpp
        src/system/hooks/local_player_turn_hook.cpp
        src/system/hooks/local_player_turn_hook.hpp
        src/events/local_player_turn_event.hpp
        src/util/exceptions.cpp
        src/system/features/modules/misc/zoom.cpp
        src/system/features/modules/misc/zoom.hpp
        src/sdk/mc/entity/components/CameraCustomFovComponent.hpp
        src/sdk/mc/deps/minecraft_camera/components/CameraRenderFirstPersonObjectsComponent.hpp
        src/sdk/mc/client/renderer/game/LevelRenderer.hpp
        src/system/hooks/get_fov_hook.cpp
        src/system/hooks/get_fov_hook.hpp
        src/sdk/mc/client/game/PlayerListener.hpp
        src/events/fov_check_event.hpp
        src/system/features/modules/misc/freecam.cpp
        src/system/features/modules/misc/freecam.hpp
        src/sdk/mc/entity/components/ActorRotationComponent.hpp
        src/sdk/mc/entity/components/RenderRotationComponent.hpp
        src/sdk/mc/world/phys/AABB.hpp
        src/sdk/mc/entity/components/AABBShapeComponent.hpp
        src/sdk/mc/entity/components/StateVectorComponent.hpp
        src/sdk/mc/entity/components/RenderPositionComponent.hpp
        src/sdk/mc/client/input/Keymapping.hpp
        src/sdk/mc/client/input/ClientInputHandler.hpp
        src/sdk/mc/deps/ecs/EntityIdType.hpp
)
# this is amazing compared to the embedding i made before lmao
include(cmake/embed_resources.cmake)
embed_resources(CppSystemsTest BACKEND ld)
embed_resources_enum(CppSystemsTest font fonts REGEX "\\.(ttf|otf|ttc)$")

target_sources(CppSystemsTest PRIVATE
        src/pch.hpp

        src/core/logging.cpp
        src/core/logging.hpp

        src/hvs/hvs.hpp

        src/system/core_system.hpp
        src/system/event_system.hpp
        src/system/complex_system.hpp
        src/system/generic_system.hpp
        src/system/systems_manager.cpp
        src/system/systems_manager.hpp

        src/events/moused_event.hpp

        src/system/features/feature_system.hpp
        src/system/features/modules/module_system.hpp

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

target_link_libraries(CppSystemsTest PRIVATE imgui::imgui glm d3d12 dxgi d3dcompiler Microsoft.GSL::GSL nlohmann_json EnTT::EnTT spdlog::spdlog kiero magic_enum libhat::libhat Boost::pfr minhook runtimeobject.lib)

set_property(TARGET CppSystemsTest spdlog kiero kiero-core magic_enum glm libhat minhook PROPERTY MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDLL>$<$<CONFIG:Release>:MultiThreaded>")

target_precompile_headers(CppSystemsTest PRIVATE "src/pch.hpp")

add_subdirectory(tools/injector)
add_dependencies(CppSystemsTest injector)