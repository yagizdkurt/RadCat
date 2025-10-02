@echo off

REM Clean up previous builds
if exist dist (
    echo Deleting existing dist folder...
    for /l %%i in (1,1,5) do echo.
    rmdir /s /q dist
)

REM Ensure dependencies
python -m pip install --upgrade pip setuptools wheel pybind11

REM Create temporary build folder to avoid clutter
set TEMP_BUILD=temp_build

if exist %TEMP_BUILD% (
    rmdir /s /q %TEMP_BUILD%
)

mkdir %TEMP_BUILD%

if exist pyt\*pyd (
    del /q pyt\*pyd
)

REM Build the module:
REM - --build-lib pyt  -> final .pyd goes to 'pyt'
REM - --build-temp temp_build -> temporary files go here
for /l %%i in (1,1,5) do echo.
echo Building the module, please wait... this may take a moment.
for /l %%i in (1,1,5) do echo.
python setup.py build_ext --build-lib pyt --build-temp temp_build 

REM > nul 2>&1


REM Clean up temporary build folder
rmdir /s /q %TEMP_BUILD%

for /l %%i in (1,1,10) do echo.
echo Module built complete. Making Executable.
pyinstaller MyProgram.spec

REM rmdir /s /q build

REM cd pyt
REM del /q YDYTU1_py.cp39-win_amd64.pyd
REM cd ..

for /l %%i in (1,1,30) do echo.
echo Everything is complete, check the 'dist' folder for output.
echo Press any key to exit.

pause