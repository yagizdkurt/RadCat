from setuptools import setup, Extension
import pybind11
from pathlib import Path
import sys

src_dir = Path(__file__).parent / "src"
libs_dir = src_dir / "libs"
# Exclude DP5 files that require LibUSB for now
excluded_files = ["DPHandler.cpp", "DP5_Usage_Example.cpp"]
cpp_sources = [str(p) for p in src_dir.rglob("*.cpp") if p.name not in excluded_files]

extra_compile_args = []
libraries = []
if sys.platform == "win32":
    extra_compile_args = ["/std:c++20","/DNOMINMAX"]  # MSVC syntax
    libraries = ["ftd2xx", "ws2_32"]  # Windows libraries
else:
    extra_compile_args = ["-std=c++20","-DNOMINMAX"]  # GCC/Clang syntax
    libraries = ["ftd2xx"]  # Unix libraries (no ws2_32 needed)

ext_modules = [
    Extension(
        "YDYTU1_py",
        cpp_sources,
        include_dirs=[
            pybind11.get_include(), 
            str(src_dir.resolve()),                    # src/
            str((src_dir / "devices").resolve()),      # src/devices/
            str((src_dir / "devices" / "comps").resolve()),  # src/devices/comps/
        ],
        libraries=libraries,
        library_dirs=[str(libs_dir.resolve())],
        language="c++",
        extra_compile_args=extra_compile_args,
    )
]

setup(
    name="YDYTU1_py",
    version="0.1.0",
    ext_modules=ext_modules,
)