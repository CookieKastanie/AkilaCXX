cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("DemoAkila")

add_subdirectory(Akila)
include_directories(Akila/include)

add_subdirectory(Demo)
