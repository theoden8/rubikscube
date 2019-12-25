cmake_minimum_required(VERSION 3.14)
project(RubiksCube)

message("C++ Compiler ${CMAKE_CXX_COMPILER}")

if(WIN32)
  set(CMAKE_CXX_FLAGS "/std:c++17")
else()
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
endif(WIN32)

set(exec rubikscube)
file(GLOB SHADERS shaders/*)
file(GLOB HEADERS *.hpp)

add_executable(${exec} ./main.cpp ${HEADERS} ${SHADERS})
include_directories($(CMAKE_CURRENT_SOURCE_DIR))

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
if(WIN32)
    include_directories(${EPOXY_INCLUDE_DIRS})
    target_link_libraries(${exec} epoxy)

    include_directories(${GLFW3_INCLUDE_DIRS})
    target_link_libraries(${exec} glfw3)
else()
  find_package(PkgConfig REQUIRED)

  pkg_check_modules(EPOXY REQUIRED IMPORTED_TARGET epoxy)
  pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
  pkg_check_modules(GLM REQUIRED IMPORTED_TARGET glm)
  target_link_libraries(${exec}
    PkgConfig::EPOXY
    PkgConfig::GLFW3
    PkgConfig::GLM
  )
endif(WIN32)

if(UNIX AND NOT APPLE)
  install(TARGETS ${exec} DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(DIRECTORY shaders DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()
