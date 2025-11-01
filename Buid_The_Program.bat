@echo off
setlocal

REM === CONFIGURATION ===
set BUILD_DIR=build
set INSTALL_DIR=dist
set GENERATOR="Ninja"
set BUILD_TYPE=Debug
set TARGET_NAME=RadCat

REM === CLEAN PREVIOUS BUILDS ===
if exist %BUILD_DIR% (
    echo Cleaning build directory...
    rmdir /s /q %BUILD_DIR%
)

if exist %INSTALL_DIR% (
    echo Cleaning install directory...
    rmdir /s /q %INSTALL_DIR%
)

REM === CONFIGURE CMAKE ===
echo.
echo Configuring project with CMake...
cmake -S . -B %BUILD_DIR% -G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM === BUILD ===
echo.
echo Building project...
cmake --build %BUILD_DIR% --config %BUILD_TYPE%

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

REM === OPTIONAL: INSTALL ===
echo.
echo Installing to %INSTALL_DIR%...
cmake --install %BUILD_DIR% --prefix %INSTALL_DIR%

REM === DONE ===
echo.
echo ==========================================
echo ✅ Build complete! Output in: %INSTALL_DIR%
echo ==========================================
echo.
pause
endlocal