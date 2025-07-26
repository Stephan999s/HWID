@echo off
echo ===============================================
echo Anti-Debug System - Build Script
echo ===============================================

REM Check if Visual Studio environment is available
where cl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Visual Studio compiler not found in PATH
    echo Please run this from Visual Studio Developer Command Prompt
    echo Or try alternative build methods:
    echo.
    echo 1. Using CMake:
    echo    mkdir build ^& cd build
    echo    cmake .. -G "Visual Studio 16 2019" -A x64
    echo    cmake --build . --config Release
    echo.
    echo 2. Using MinGW:
    echo    g++ -std=c++17 -O2 main.cpp -o AntiDebugSystem.exe -lkernel32 -luser32 -lntdll -ladvapi32
    pause
    exit /b 1
)

REM Clean previous builds
if exist AntiDebugSystem.exe del AntiDebugSystem.exe
if exist *.obj del *.obj
if exist *.pdb del *.pdb

echo Building Anti-Debug System...
echo.

REM Compile with MSVC
cl /EHsc /O2 /std:c++17 /W3 ^
   /D_CRT_SECURE_NO_WARNINGS ^
   /D_WIN32_WINNT=0x0601 ^
   /DWINVER=0x0601 ^
   /DWIN32_LEAN_AND_MEAN ^
   /DNOMINMAX ^
   main.cpp ^
   /link ^
   kernel32.lib ^
   user32.lib ^
   ntdll.lib ^
   advapi32.lib ^
   ole32.lib ^
   shell32.lib ^
   winmm.lib ^
   /SUBSYSTEM:CONSOLE ^
   /OUT:AntiDebugSystem.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo ===============================================
    echo Build successful!
    echo Output: AntiDebugSystem.exe
    echo ===============================================
    echo.
    echo To run the program:
    echo   AntiDebugSystem.exe
    echo.
    echo Note: Some anti-debug features may require
    echo       administrator privileges.
) else (
    echo.
    echo ===============================================
    echo Build failed!
    echo ===============================================
    echo.
    echo Check the error messages above for details.
    echo Common issues:
    echo - Missing Windows SDK
    echo - Incorrect Visual Studio setup
    echo - Missing required libraries
)

REM Clean intermediate files
if exist *.obj del *.obj

echo.
pause