#pragma once
#include <vector>
#include "Ingrediente.hpp"
#include "Pocao.hpp"

class Alquimista {
public:
    double ouro = 100.0;
    int reputacao = 10;
    std::vector<Ingrediente> inventario;
    std::vector<Pocao> colecaoPocoes;

    void inicializarInventarioInicial();
};