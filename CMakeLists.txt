cmake_minimum_required(VERSION 3.20)
project(O_Mago_Alquimista CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Lista explícita de todos os arquivos .cpp
set(SOURCES
        src/Alquimista.cpp
        src/Caldeirao.cpp
        src/GeradorProcedural.cpp
        src/Mercado.cpp
        src/main.cpp
        src/Mercado.cpp
)

add_executable(MagoAlquimista ${SOURCES})

# Adicionar os headers também para visibilidade no IDE
target_sources(MagoAlquimista PRIVATE
        src/Alquimista.hpp
        src/Caldeirao.hpp
        src/GeradorProcedural.hpp
        src/Ingrediente.hpp
        src/Mercado.hpp
        src/Pocao.hpp
        src/Tipos.hpp
)