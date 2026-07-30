// src/GeradorProcedural.hpp
#pragma once
#include "Ingrediente.hpp"
#include <random>
#include <vector>
#include <string>
#include <unordered_map>

class GeradorProcedural {
private:
    // Prefixos organizados por raridade
    std::vector<std::string> prefixosComuns = {
        "Silvestre", "Pequeno", "Seco", "Humilde", "Puro", "Fresco",
        "Simples", "Básico", "Comum", "Natural", "Selvagem"
    };

    std::vector<std::string> prefixosRaros = {
        "Radiante", "Ancestral", "Lunar", "Flutuante", "Vulcânico",
        "Místico", "Sagrado", "Antigo", "Dourado", "Prateado"
    };

    std::vector<std::string> prefixosLendarios = {
        "Astral", "Etéreo", "Abissal", "Cósmico", "Cronos",
        "Divino", "Primordial", "Infinito", "Eterno", "Supremo"
    };

    // Substantivos (categorias de ingredientes)
    std::vector<std::string> substantivos = {
        "Flor", "Raiz", "Cogumelo", "Escama", "Lótus",
        "Cristal", "Orvalho", "Pó", "Garra", "Folha",
        "Semente", "Pétala", "Casca", "Essência", "Néctar",
        "Fragmento", "Pergaminho", "Chama", "Gota", "Lágrima"
    };

    // Sufixos (modificadores secundários)
    std::vector<std::string> sufixos = {
        "da Alvorada", "do Crepúsculo", "dos Dragões",
        "do Vazio", "da Serenidade", "da Morte",
        "da Vida", "do Caos", "da Ordem", "do Tempo",
        "das Estrelas", "do Abismo", "da Eternidade"
    };

    // Geradores de números aleatórios
    std::mt19937 rng;
    std::uniform_real_distribution<float> distFloat{0.0f, 1.0f};
    std::uniform_int_distribution<int> distInt{0, 100};

    // Mapeamento de raridade para multiplicadores de atributo
    std::unordered_map<int, float> multiplicadorRaridade = {
        {1, 1.0f},  // Comum
        {2, 1.5f},  // Incomum
        {3, 2.5f},  // Raro
        {4, 4.0f},  // Épico
        {5, 6.5f}   // Lendário
    };

    // Métodos privados
    std::string getPrefixPorRaridade(int raridade);
    std::unordered_map<AtributoMagico, float> gerarPropriedades(int raridade);
    AtributoMagico getAtributoAleatorio();
    float gerarValorAtributo(int raridade, bool primario = true);

public:
    GeradorProcedural();

    // Método principal para gerar ingrediente
    Ingrediente gerarIngrediente(int nivelJogador);

    // Gera múltiplos ingredientes de uma vez
    std::vector<Ingrediente> gerarLoteDeIngredientes(int nivelJogador, int quantidade);

    // Gera ingrediente com raridade específica
    Ingrediente gerarIngredienteComRaridade(int raridade);

    // Gera ingrediente com atributo específico dominante
    Ingrediente gerarIngredienteComAtributo(AtributoMagico atributo, int raridade);

    // Métodos auxiliares
    std::string gerarNomeIngrediente(int raridade);
    float calcularPrecoBase(int raridade, const std::unordered_map<AtributoMagico, float>& propriedades);
};