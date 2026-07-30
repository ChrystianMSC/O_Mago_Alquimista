#include "Caldeirao.hpp"
#include <unordered_map>

Pocao Caldeirao::misturar(const std::vector<Ingrediente>& ingredientes) {
    Pocao pocaoResultante;
    if (ingredientes.empty()) return pocaoResultante;

    std::unordered_map<AtributoMagico, float> somaAtributos;
    float somaA = 0.0f;

    for (const auto& ing : ingredientes) {
        float fatorMult = ing.ehPocaoReciclada ? 0.8f : 1.0f;
        for (const auto& [attr, valor] : ing.propriedades) {
            float valorAjustado = valor * fatorMult;
            somaAtributos[attr] += valorAjustado;
            somaA += valorAjustado;
        }
    }

    if (somaA <= 0.0f) return pocaoResultante;

    AtributoMagico dominante = AtributoMagico::HEAL;
    float maiorValor = -1.0f;

    for (const auto& [attr, valor] : somaAtributos) {
        if (valor > maiorValor) {
            maiorValor = valor;
            dominante = attr;
        }
    }

    float pureza = maiorValor / somaA;
    float potencia = maiorValor * pureza;
    float valorComercial = (potencia * 2.5f * pureza) + (somaA * 0.5f);

    pocaoResultante.nome = "Potion of " + attributeToString(dominante);
    pocaoResultante.efeitoDominante = dominante;
    pocaoResultante.potencia = potencia;
    pocaoResultante.pureza = pureza;
    pocaoResultante.valorComercial = valorComercial;
    pocaoResultante.receitaUtilizada = ingredientes;

    return pocaoResultante;
}