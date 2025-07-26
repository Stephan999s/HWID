cmake_minimum_required(VERSION 3.16)
project(AntiDebugSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Windows target version
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7 and later
    add_definitions(-DWINVER=0x0601)
    add_definitions(-DNTDDI_VERSION=0x06010000)
endif()

# Compiler specific options
if(MSVC)
    # MSVC compiler options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /DEBUG")
    
    # Enable exception handling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    
    # Disable some warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")  # Disable deprecated warnings
    
    # Security features
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS /sdl")
    
    # Link with required libraries
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang compiler options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    
    if(WIN32)
        # MinGW specific flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

# Source files
set(SOURCES
    main.cpp
    include.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Windows specific libraries and settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        kernel32
        user32
        ntdll
        advapi32
        ole32
        shell32
        winmm
    )
    
    # Set subsystem to console
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
    endif()
    
    # Add version info (optional)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION 1.0.0.0
        SOVERSION 1
    )
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# Debug/Release specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Optional: Copy to specific output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Optional: Create build info
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/build_info.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/build_info.h"
    @ONLY
)