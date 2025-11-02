@echo off
setlocal

REM === CONFIGURATION ===
set BUILD_DIR=build
set INSTALL_DIR=dist
set GENERATOR="Ninja"
set BUILD_TYPE=Debug
set TARGET_NAME=RadCat

REM === SETUP MSVC ENVIRONMENT (so link.exe can find MSVCRTD.lib) ===
REM Check for vcvars and call it (avoid multi-line parenthesis blocks to prevent parsing issues)
if exist "D:\VS\VC\Auxiliary\Build\vcvarsall.bat" goto :vcvars_found
echo Warning: vcvarsall.bat not found at D:\VS\VC\Auxiliary\Build\vcvarsall.bat
echo If you have Visual Studio installed in a different location, update this script
echo or run the "x64 Native Tools Command Prompt for VS" before running this batch.
goto :vcvars_done

:vcvars_found
echo Setting up Visual Studio build environment (vcvarsall)...
call "D:\VS\VC\Auxiliary\Build\vcvarsall.bat" amd64 >nul

:vcvars_done

REM === CLEAN PREVIOUS BUILDS ===
if exist %BUILD_DIR% (
    echo Cleaning build directory...
    rmdir /s /q %BUILD_DIR%
)

echo [DEBUG] After cleaning build dir

if exist %INSTALL_DIR% (
    echo Cleaning install directory...
    rmdir /s /q %INSTALL_DIR%
)

REM === CONFIGURE CMAKE ===
echo.
echo [1/2] Configuring project with CMake...
cmake -S . -B %BUILD_DIR% -G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% >nul
echo Done.

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM === BUILD ===
echo.
echo [2/2] Building project...
cmake --build %BUILD_DIR% --config %BUILD_TYPE% 
echo Done.

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

REM === DONE ===
echo.
echo ==========================================
echo Build complete! Output in: %INSTALL_DIR%
echo ==========================================
echo.
pause
endlocal