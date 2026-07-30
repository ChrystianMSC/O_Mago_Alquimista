#pragma once
#include <string>
#include <vector>
#include "Tipos.hpp"
#include "Ingrediente.hpp"

struct Pocao {
    std::string nome = "Murky Water";
    AtributoMagico efeitoDominante = AtributoMagico::HEAL;
    float potencia = 0.0f;
    float pureza = 0.0f;
    float valorComercial = 0.0f;
    std::string asciiArtGarrafa;
    std::vector<Ingrediente> receitaUtilizada;
};