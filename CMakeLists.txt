# ---- Project And C++ ----
cmake_minimum_required(VERSION 3.21)
project(RadCat VERSION 0.1 LANGUAGES CXX)

# PIE (Position Independent Executable) zorunluluğunu kaldır
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Output directories ----
set(DIST_DIR "${CMAKE_SOURCE_DIR}/dist")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${cfg}" CFG_UP)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_UP} "${DIST_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_UP} "${DIST_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_UP} "${DIST_DIR}/lib")
endforeach()

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
    ${CMAKE_SOURCE_DIR}/src/Components
    ${CMAKE_SOURCE_DIR}/src/devices
    ${CMAKE_SOURCE_DIR}/src/devices/deviceCoreSystems
    ${CMAKE_SOURCE_DIR}/src/CompHandlers
    ${CMAKE_SOURCE_DIR}/src/Included
    
    
)

# ---- Get Platform and platform specific includes ----
# ---- Platform and platform specific includes (DÜZELTİLMİŞ TAM BLOK) ----

if (WIN32)
    message(STATUS "Platform: Windows Detected")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Included/Windows)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/libs/ftd2xx.lib) 
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)

elseif (APPLE)
    message(STATUS "Platform: macOS Detected")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Included/Mac)
    
    find_library(FTDI_LIB_MAC ftd2xx HINTS "${CMAKE_SOURCE_DIR}/src/libs" "/usr/local/lib" "/opt/homebrew/lib")
    if(FTDI_LIB_MAC)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${FTDI_LIB_MAC})
    else()
        message(FATAL_ERROR "FTDI Library not found for macOS! Please install drivers.")
    endif()

else()
    # --- LINUX KISMI (BURAYI GARANTİ HALE GETİRDİK) ---
    message(STATUS "Platform: Linux Detected")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Included/Linux)
    
    # 1. FTDI Kütüphanesini "İthal Hedef" (Imported Target) olarak tanımla
    # Bu yöntemle CMake dosyayı bulamama ihtimalini ortadan kaldırır.
    add_library(FTDI_STATIC STATIC IMPORTED)

    # 2. Dosyanın yerini KESİN olarak göster
    set_target_properties(FTDI_STATIC PROPERTIES 
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/src/Included/Linux/libftd2xx.a"
    )

    # 3. Bağlama İşlemi (Önce bizim hedef, sonra sistem kütüphaneleri)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        FTDI_STATIC
        pthread
        dl
        usb-1.0
    )

endif()




# ---- Dependencies / LIBRARIES ----

# ---- Post-build: copy FTDI runtime on Windows into dist ----
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Dlls/FTD2XX.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/FTD2XX.dll")
endif()

## --- Qt ---
    set(QT_REQUIRED_COMPONENTS Core Gui Widgets)
    find_package(Qt6 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

   ## --- ROOT ---
   find_package(ROOT REQUIRED)
   target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE_DIRS})
   target_link_libraries(${PROJECT_NAME} PRIVATE ${ROOT_LIBRARIES})

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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # IDE
find_library(FTDI_LIB ftd2xx PATHS /usr/local/lib NO_DEFAULT_PATH)
