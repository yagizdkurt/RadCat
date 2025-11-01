cmake_minimum_required(VERSION 3.21)

# ---- Project ----
project(RadCat
    VERSION 0.1
    LANGUAGES CXX
)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Output directories ----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---- Collect all source files recursively ----
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ---- Platform-specific definitions ----
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif()

# ---- Dependencies ----

# --- Qt ---
set(QT_REQUIRED_COMPONENTS Core Gui Widgets)
find_package(Qt6 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# --- ROOT ---
find_package(ROOT REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ROOT_LIBRARIES})

# --- FTDI ---
find_path(FTDI_INCLUDE_DIR ftd2xx.h PATHS "C:/Program Files/FTDI" "C:/Program Files (x86)/FTDI")
find_library(FTDI_LIBRARY NAMES ftd2xx libftdi PATHS "C:/Program Files/FTDI" "C:/Program Files (x86)/FTDI")

if (FTDI_INCLUDE_DIR AND FTDI_LIBRARY)
    message(STATUS "Found FTDI: ${FTDI_LIBRARY}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${FTDI_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FTDI_LIBRARY})
else()
    message(WARNING "FTDI library not found! Please set FTDI_INCLUDE_DIR and FTDI_LIBRARY manually.")
endif()

# ---- Compiler warnings ----
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# ---- Enable IDE support ----
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Cross-platform install step ----
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .       # For .exe on Windows
    LIBRARY DESTINATION .       # For .so on Linux
    ARCHIVE DESTINATION .       # For static .a
)


# ---- Platform-specific install steps ----
if (WIN32)
    message(STATUS "Windows install: copying Qt, ROOT, and FTDI DLLs if found...")

    # --- Qt Deployment (Windows) ---
    # Use windeployqt if Qt is found and available
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${QT_INSTALL_PREFIX}/bin)
    if (WINDEPLOYQT_EXECUTABLE)
        install(CODE "
            message(STATUS \"Running windeployqt for Qt runtime dependencies...\")
            execute_process(
                COMMAND \"${WINDEPLOYQT_EXECUTABLE}\" \"$<TARGET_FILE:${PROJECT_NAME}>\"
                WORKING_DIRECTORY \"$<TARGET_FILE_DIR:${PROJECT_NAME}>\"
            )
        ")
    else()
        message(WARNING "windeployqt not found — Qt DLLs may need to be copied manually.")
    endif()

    # --- ROOT libraries ---
    if (ROOT_LIBRARIES)
        message(STATUS "Installing ROOT DLLs...")
        install(FILES ${ROOT_LIBRARIES} DESTINATION . OPTIONAL)
    endif()

    # --- FTDI libraries ---
    if (FTDI_LIBRARY)
        get_filename_component(FTDI_DIR "${FTDI_LIBRARY}" DIRECTORY)
        file(GLOB FTDI_DLLS "${FTDI_DIR}/*.dll")
        if (FTDI_DLLS)
            message(STATUS "Installing FTDI DLLs...")
            install(FILES ${FTDI_DLLS} DESTINATION .)
        endif()
    endif()

elseif(APPLE)
    message(STATUS "macOS install: deploying app bundle and dependencies...")

    # --- Qt Deployment (macOS) ---
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS ${QT_INSTALL_PREFIX}/bin)
    if (MACDEPLOYQT_EXECUTABLE)
        install(CODE "
            message(STATUS \"Running macdeployqt...\")
            execute_process(
                COMMAND \"${MACDEPLOYQT_EXECUTABLE}\" \"$<TARGET_FILE:${PROJECT_NAME}>\"
                WORKING_DIRECTORY \"$<TARGET_FILE_DIR:${PROJECT_NAME}>\"
            )
        ")
    endif()

    # --- ROOT / FTDI ---
    # macOS uses .dylib; copying not always required, handled by @rpath usually.
    message(STATUS "macOS: ROOT/FTDI libs resolved by @rpath.")

elseif(UNIX)
    message(STATUS "Linux install: installing shared libraries (.so) if needed...")

    # --- ROOT libraries ---
    if (ROOT_LIBRARIES)
        install(FILES ${ROOT_LIBRARIES} DESTINATION lib OPTIONAL)
    endif()

    # --- FTDI libraries ---
    if (FTDI_LIBRARY)
        get_filename_component(FTDI_DIR "${FTDI_LIBRARY}" DIRECTORY)
        file(GLOB FTDI_SOS "${FTDI_DIR}/*.so")
        install(FILES ${FTDI_SOS} DESTINATION lib OPTIONAL)
    endif()

    message(STATUS "Linux: use LD_LIBRARY_PATH or rpath for runtime resolution.")
endif()