#pragma once
#include <string>
#include <unordered_map>
#include "Tipos.hpp"

struct Ingrediente {
    std::string id;
    std::string nome;
    int raridade = 1; // 1 a 5
    float precoBase = 10.0f;
    std::unordered_map<AtributoMagico, float> propriedades;
    bool ehPocaoReciclada = false;
};