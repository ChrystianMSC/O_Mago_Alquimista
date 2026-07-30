// src/Mercado.hpp
#pragma once
#include "Ingrediente.hpp"
#include "Alquimista.hpp"
#include "GeradorProcedural.hpp"
#include <vector>
#include <memory>

struct Encomenda {
    std::string descricao;
    std::unordered_map<AtributoMagico, float> requisitosMinimos;
    std::unordered_map<AtributoMagico, float> requisitosMaximos; // Opcional
    float recompensaOuro;
    int recompensaReputacao;
    std::string clienteNome;
    bool cumprida = false;
};

class Mercado {
private:
    std::vector<Ingrediente> estoqueIngredientes;
    std::vector<Encomenda> encomendasAtivas;
    std::unique_ptr<GeradorProcedural> gerador;
    std::mt19937 rng;

    // Constantes de economia
    static constexpr int MAX_INGREDIENTES_ESTOQUE = 12;
    static constexpr int MAX_ENCOMENDAS = 5;
    static constexpr float MARGEM_VENDA = 1.4f; // 40% de margem

    // Métodos privados
    void gerarEstoqueDiario(int nivelJogador);
    void gerarEncomendas(int nivelJogador);
    Encomenda criarEncomendaAleatoria(int nivelJogador);

public:
    Mercado();

    // Inicialização diária
    void iniciarNovoDia(int nivelJogador);

    // Visualização
    void exibirEstoque() const;
    void exibirEncomendas() const;

    // Transações
    bool comprarIngrediente(Alquimista& jogador, int index);
    bool venderPocao(Alquimista& jogador, int indexPocao);
    bool atenderEncomenda(Alquimista& jogador, int indexPocao, int indexEncomenda);

    // Getters
    const std::vector<Ingrediente>& getEstoque() const { return estoqueIngredientes; }
    const std::vector<Encomenda>& getEncomendas() const { return encomendasAtivas; }

    // Métodos auxiliares
    static std::string requisitosParaString(const Encomenda& encomenda);
};