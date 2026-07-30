#pragma once
#include <vector>
#include "Ingrediente.hpp"
#include "Pocao.hpp"

class Caldeirao {
public:
    static Pocao misturar(const std::vector<Ingrediente>& ingredientes);
};