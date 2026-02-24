
set(CMAKE_CXX_STANDARD 11) # or higher

add_library(ZACLib STATIC "ZACLib.cpp") # if use source
target_include_directories(ZACLib PUBLIC ".")
set_target_properties(ZACLib PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)