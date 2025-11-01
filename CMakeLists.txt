# ---- Project And C++ ----
cmake_minimum_required(VERSION 3.21)
project(RadCat VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Output directories ----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---- Collect all source files ----
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

# ---- Create executable target ----
add_executable(${PROJECT_NAME} ${SRC_FILES})

# ---- Include directories ----
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/devices
    ${CMAKE_SOURCE_DIR}/src/devices/comps
)

# ---- Get Platform ----
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif()

# ---- Dependencies / LIBRARIES ----

    ## --- Qt ---
    set(QT_REQUIRED_COMPONENTS Core Gui Widgets)
    find_package(Qt6 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

    ## --- ROOT ---
    find_package(ROOT REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${ROOT_LIBRARIES})

    ## --- FTDI ---
    # Set paths relative to project
    set(FTDI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")
    set(FTDI_LIBRARY "${CMAKE_SOURCE_DIR}/src/libs/ftd2xx.lib")  # Use .lib if on MSVC

    if(EXISTS "${FTDI_INCLUDE_DIR}/ftd2xx.h" AND EXISTS "${FTDI_LIBRARY}")
        message(STATUS "Found FTDI library: ${FTDI_LIBRARY}")
        target_include_directories(${PROJECT_NAME} PRIVATE ${FTDI_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${FTDI_LIBRARY})

        # Copy DLL to executable folder after build
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/DLLs/ftd2xx.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    else()
        message(WARNING "FTDI library not found! Please check src/libs/ftd2xx.lib and src/ftd2xx.h")
    endif()

    ## --- Windows-specific libraries ---
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    endif()

# ---- Compiler warnings ----
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4100 /wd4189)
    # /wd4100 -> unreferenced formal parameter
    # This happens when a function has a parameter that is never used in its body.
    # /wd4189 -> local variable is initialized but not referenced
    # This happens when you create a local variable, give it a value, but never use it.
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-variable)
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

    ## --- Qt Deployment (Windows) ---
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

    ## --- ROOT libraries ---
    if (ROOT_LIBRARIES)
        message(STATUS "Installing ROOT DLLs...")
        install(FILES ${ROOT_LIBRARIES} DESTINATION . OPTIONAL)
    endif()

    ## --- FTDI libraries ---
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

    ## --- Qt Deployment (macOS) ---
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

    ## --- ROOT / FTDI ---
    # macOS uses .dylib; copying not always required, handled by @rpath usually.
    message(STATUS "macOS: ROOT/FTDI libs resolved by @rpath.")

elseif(UNIX)
    message(STATUS "Linux install: installing shared libraries (.so) if needed...")

    ## --- ROOT libraries ---
    if (ROOT_LIBRARIES)
        install(FILES ${ROOT_LIBRARIES} DESTINATION lib OPTIONAL)
    endif()

    ## --- FTDI libraries ---
    if (FTDI_LIBRARY)
        get_filename_component(FTDI_DIR "${FTDI_LIBRARY}" DIRECTORY)
        file(GLOB FTDI_SOS "${FTDI_DIR}/*.so")
        install(FILES ${FTDI_SOS} DESTINATION lib OPTIONAL)
    endif()

    message(STATUS "Linux: use LD_LIBRARY_PATH or rpath for runtime resolution.")
endif()