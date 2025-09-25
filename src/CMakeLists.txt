file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(YDYTU1_lib ${SRC_FILES})
target_compile_features(YDYTU1_lib PUBLIC cxx_std_17)




