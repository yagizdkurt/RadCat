# -*- mode: python ; coding: utf-8 -*-
import os
import glob
import sys
from pathlib import Path
from PyInstaller.utils.hooks import collect_submodules

# Paths
spec_dir = os.getcwd()
python_program_folder = os.path.join(spec_dir, 'pyt')
main_script = os.path.join(python_program_folder, 'main.py')
parent_folder = spec_dir  # Output folder for EXE
print(f"[PyInstaller SPEC] Output distpath will be: {parent_folder}")

block_cipher = None

# --- Automatically detect MSVC runtime DLLs ---
def find_msvc_dlls():
    """
    Find the MSVC runtime DLLs for the current system.
    Looks in System32 and SysWOW64 depending on architecture.
    """
    dll_names = ["vcruntime140.dll", "msvcp140.dll", "ucrtbase.dll"]
    system_dir = os.path.join(os.environ['SystemRoot'], 'System32')
    if sys.maxsize <= 2**32:
        # 32-bit Python on 64-bit Windows
        system_dir = os.path.join(os.environ['SystemRoot'], 'SysWOW64')

    found_dlls = []
    for dll in dll_names:
        path = os.path.join(system_dir, dll)
        if os.path.exists(path):
            found_dlls.append(path)
        else:
            print(f"[WARNING] Could not find {dll} in {system_dir}")
    return found_dlls

msvc_dlls = find_msvc_dlls()
binaries = [(dll, '.') for dll in msvc_dlls]  # Copy next to EXE
binaries.append((str((Path(spec_dir) / "src" / "libs" / "FTD2XX64.dll").resolve()), '.'))


# --- Analysis ---
a = Analysis(
    [main_script],
    pathex=[python_program_folder],
    binaries=binaries,        # include MSVC DLLs
    datas=[],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)

pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='TextExe',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='YDPT_Test',
    distpath=spec_dir
)
