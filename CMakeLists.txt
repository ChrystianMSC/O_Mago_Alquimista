cmake_minimum_required(VERSION 3.20)
project(O_Mago_Alquimista CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

file(GLOB SOURCES "src/*.cpp")

add_executable(MagoAlquimista ${SOURCES}
        src/Tipos.hpp
        src/Ingrediente.hpp
        src/Pocao.hpp
        src/Alquimista.hpp
        src/Alquimista.cpp
        src/Caldeirao.hpp
        src/Caldeirao.cpp)