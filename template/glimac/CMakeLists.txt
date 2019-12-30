include_directories(include)
include_directories(../third-party/include/)
file(GLOB_RECURSE SRC_FILES *.cpp *.hpp)
add_library(glimac ${SRC_FILES})
