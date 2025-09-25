from setuptools import setup, Extension
import pybind11
from pathlib import Path

# Collect all .cpp files in src/
sources = [str(p) for p in Path("src").rglob("*.cpp")]

ext_modules = [
    Extension(
        "your_module",      # Python import name
        sources,            # all sources under src/
        include_dirs=[pybind11.get_include()],
        language="c++",
        extra_compile_args=["-std=c++17"],
    ),
]

setup(
    name="your_module",
    version="0.1.0",
    author="Your Name",
    description="C++ classes exposed to Python via pybind11",
    ext_modules=ext_modules,
    zip_safe=False,
)