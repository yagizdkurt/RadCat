@echo off
REM Go to the directory where this script is located
cd /d "%~dp0"

if not exist build (
    mkdir build
)

cd build

cmake ..

cmake --build . --config Debug


echo.
echo.
echo.
echo.
echo.
echo file is ready, press enter to close.
pause