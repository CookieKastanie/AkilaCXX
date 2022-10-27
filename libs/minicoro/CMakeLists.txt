project(minicoro)

cmake_minimum_required(VERSION 3.0.2)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC  
  ${SRC_DIR}/minicoro.cpp
)

set(HEADER
  ${INC_DIR}/${PROJECT_NAME}/minicoro.h
)

add_library(${PROJECT_NAME} ${SRC} ${HEADER})
target_include_directories (${PROJECT_NAME}
  PUBLIC ${INC_DIR}
)

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
