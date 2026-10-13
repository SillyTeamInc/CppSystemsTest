add_executable(injector injector.cpp)

set_target_properties(injector PROPERTIES
        UNITY_BUILD OFF
        MSVC_RUNTIME_LIBRARY MultiThreaded
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(injector PRIVATE kernel32)