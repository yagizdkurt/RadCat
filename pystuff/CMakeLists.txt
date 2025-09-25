# Python wrapper module
pybind11_add_module(YDYTU1_py wrapper.cpp)

# Link to the core library
target_link_libraries(YDYTU1_py PRIVATE YDYTU1_lib)

# Include the core headers
target_include_directories(YDYTU1_py PRIVATE "../src")

# Optional MSVC flag
if(MSVC)
    target_compile_options(YDYTU1_py PRIVATE /Zc:__cplusplus)
endif()

set_target_properties(YDYTU1_py
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "C:/Users/WWW/Desktop/YDPT/PythonProgram"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "C:/Users/WWW/Desktop/YDPT/PythonProgram"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "C:/Users/WWW/Desktop/YDPT/PythonProgram"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "C:/Users/WWW/Desktop/YDPT/PythonProgram"
)