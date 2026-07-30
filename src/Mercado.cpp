// src/Mercado.cpp
#include "Mercado.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>

Mercado::Mercado()
    : gerador(std::make_unique<GeradorProcedural>()),
      rng(std::random_device{}()) {
    // Inicialização vazia - espera chamada de iniciarNovoDia
}

void Mercado::iniciarNovoDia(int nivelJogador) {
    gerarEstoqueDiario(nivelJogador);
    gerarEncomendas(nivelJogador);
}

void Mercado::gerarEstoqueDiario(int nivelJogador) {
    estoqueIngredientes.clear();

    // Gerar quantidade de ingredientes (8-12)
    std::uniform_int_distribution<int> distQtd(8, MAX_INGREDIENTES_ESTOQUE);
    int quantidade = distQtd(rng);

    // Gerar ingredientes
    for (int i = 0; i < quantidade; ++i) {
        // Nível do jogador influencia a qualidade dos ingredientes disponíveis
        int nivelAjustado = nivelJogador + (static_cast<int>(rng()) % 3 - 1); // +- 1 nível
        Ingrediente ing = gerador->gerarIngrediente(std::max(1, nivelAjustado));
        estoqueIngredientes.push_back(ing);
    }

    // Ordenar por raridade (do mais raro para o mais comum)
    std::sort(estoqueIngredientes.begin(), estoqueIngredientes.end(),
        [](const Ingrediente& a, const Ingrediente& b) {
            return a.raridade > b.raridade;
        });
}

void Mercado::gerarEncomendas(int nivelJogador) {
    encomendasAtivas.clear();

    // Número de encomendas (2-5)
    std::uniform_int_distribution<int> distQtd(2, MAX_ENCOMENDAS);
    int quantidade = distQtd(rng);

    for (int i = 0; i < quantidade; ++i) {
        encomendasAtivas.push_back(criarEncomendaAleatoria(nivelJogador));
    }
}

// Função auxiliar para gerar float aleatório entre 0 e 1
static float randomFloat(std::mt19937& rng) {
    return std::uniform_real_distribution<float>(0.0f, 1.0f)(rng);
}

Encomenda Mercado::criarEncomendaAleatoria(int nivelJogador) {
    Encomenda encomenda;

    // Nomes de clientes
    std::vector<std::string> clientes = {
        "Guilda dos Cavaleiros", "Conselho dos Magos", "Irmandade das Sombras",
        "Ordem da Luz", "Círculo Druídico", "Alto Conselho Élfico",
        "Guilda dos Mercadores", "Mosteiro dos Sábios", "Tribo dos Ventos"
    };

    std::uniform_int_distribution<int> distCliente(0, clientes.size() - 1);
    encomenda.clienteNome = clientes[distCliente(rng)];

    // Escolher atributo foco
    AtributoMagico atributoPrincipal = static_cast<AtributoMagico>(
        std::uniform_int_distribution<int>(0, 7)(rng)
    );

    // Nível de dificuldade baseado no nível do jogador
    std::uniform_int_distribution<int> distDificuldade(
        std::max(1, nivelJogador - 1),
        nivelJogador + 2
    );
    int dificuldade = distDificuldade(rng);
    dificuldade = std::max(1, std::min(5, dificuldade));

    // Requisitos mínimos
    float valorRequisito = 10.0f + dificuldade * 8.0f;
    encomenda.requisitosMinimos[atributoPrincipal] = valorRequisito;

    // 30% de chance de ter um segundo requisito
    if (randomFloat(rng) < 0.3f) {
        AtributoMagico segundoAtributo;
        do {
            segundoAtributo = static_cast<AtributoMagico>(
                std::uniform_int_distribution<int>(0, 7)(rng)
            );
        } while (segundoAtributo == atributoPrincipal);

        encomenda.requisitosMinimos[segundoAtributo] = valorRequisito * 0.6f;
    }

    // Requisitos máximos (ex: não pode ter veneno)
    if (randomFloat(rng) < 0.4f) {
        AtributoMagico atributoRestrito = static_cast<AtributoMagico>(
            std::uniform_int_distribution<int>(0, 7)(rng)
        );
        // Evitar conflito com requisitos mínimos
        if (encomenda.requisitosMinimos.find(atributoRestrito) ==
            encomenda.requisitosMinimos.end()) {
            encomenda.requisitosMaximos[atributoRestrito] = 0.0f; // Não pode ter
        }
    }

    // Gerar descrição
    encomenda.descricao = "Encomenda de " + encomenda.clienteNome +
                         ": Poção com " + attributeToString(atributoPrincipal) +
                         " mínimo " + std::to_string(static_cast<int>(valorRequisito));

    if (!encomenda.requisitosMaximos.empty()) {
        for (const auto& [attr, val] : encomenda.requisitosMaximos) {
            encomenda.descricao += ", sem " + attributeToString(attr);
        }
    }

    // Calcular recompensa
    float recompensaBase = valorRequisito * 15.0f;
    encomenda.recompensaOuro = recompensaBase * (1.0f + dificuldade * 0.1f);
    encomenda.recompensaOuro = std::round(encomenda.recompensaOuro * 100.0f) / 100.0f;

    encomenda.recompensaReputacao = 5 + dificuldade * 3;

    return encomenda;
}

void Mercado::exibirEstoque() const {
    if (estoqueIngredientes.empty()) {
        std::cout << "  O estoque do mercado está vazio hoje.\n";
        return;
    }

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              🏪 MERCADO - INGREDIENTES DISPONÍVEIS           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    for (size_t i = 0; i < estoqueIngredientes.size(); ++i) {
        const auto& ing = estoqueIngredientes[i];

        // Cor baseada na raridade
        std::string corRaridade;
        if (ing.raridade <= 2) corRaridade = Color::GRAY;
        else if (ing.raridade <= 3) corRaridade = Color::BLUE;
        else if (ing.raridade <= 4) corRaridade = Color::PURPLE;
        else corRaridade = Color::YELLOW;

        std::cout << "  " << i + 1 << ". "
                  << corRaridade << ing.nome << Color::RESET
                  << " [R" << ing.raridade << "]"
                  << " - " << Color::YELLOW << ing.precoBase << " GP" << Color::RESET;

        // Mostrar atributos
        std::cout << " (";
        bool first = true;
        for (const auto& [attr, val] : ing.propriedades) {
            if (!first) std::cout << ", ";
            std::cout << attributeToString(attr) << " +" << val;
            first = false;
        }
        std::cout << ")\n";
    }
    std::cout << "\n";
}

void Mercado::exibirEncomendas() const {
    if (encomendasAtivas.empty()) {
        std::cout << "  Não há encomendas ativas no momento.\n";
        return;
    }

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               📜 ENCOMENDAS DA GUILDA                       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    for (size_t i = 0; i < encomendasAtivas.size(); ++i) {
        const auto& enc = encomendasAtivas[i];
        if (enc.cumprida) continue;

        std::cout << "\n  " << i + 1 << ". " << enc.descricao << "\n";
        std::cout << "     Recompensa: " << Color::YELLOW << enc.recompensaOuro
                  << " GP" << Color::RESET << " e "
                  << Color::GREEN << "+" << enc.recompensaReputacao
                  << " Reputação" << Color::RESET << "\n";
    }
    std::cout << "\n";
}

bool Mercado::comprarIngrediente(Alquimista& jogador, int index) {
    if (index < 0 || index >= static_cast<int>(estoqueIngredientes.size())) {
        std::cout << "  Índice inválido!\n";
        return false;
    }

    const Ingrediente& ing = estoqueIngredientes[index];

    if (jogador.ouro < ing.precoBase) {
        std::cout << "  Ouro insuficiente! Você tem " << jogador.ouro
                  << " GP, precisa de " << ing.precoBase << " GP.\n";
        return false;
    }

    // Efetuar compra
    jogador.ouro -= ing.precoBase;
    jogador.inventario.push_back(ing);

    // Remover do estoque
    estoqueIngredientes.erase(estoqueIngredientes.begin() + index);

    std::cout << "  Compra realizada com sucesso! "
              << Color::GREEN << ing.nome << Color::RESET
              << " adicionado ao inventário.\n";

    return true;
}

bool Mercado::venderPocao(Alquimista& jogador, int indexPocao) {
    if (indexPocao < 0 || indexPocao >= static_cast<int>(jogador.colecaoPocoes.size())) {
        std::cout << "  Índice de poção inválido!\n";
        return false;
    }

    const Pocao& pocao = jogador.colecaoPocoes[indexPocao];

    // Preço de venda (mercado paga menos que o valor comercial)
    float precoVenda = pocao.valorComercial * 0.7f;

    // Bônus de reputação para poções de alta pureza
    if (pocao.pureza > 0.9f) {
        precoVenda *= 1.2f;
        std::cout << "  ⭐ Bônus por pureza excepcional!\n";
    }

    precoVenda = std::round(precoVenda * 100.0f) / 100.0f;

    // Efetuar venda
    jogador.ouro += precoVenda;
    jogador.colecaoPocoes.erase(jogador.colecaoPocoes.begin() + indexPocao);

    std::cout << "  Poção vendida por " << Color::YELLOW << precoVenda
              << " GP" << Color::RESET << "!\n";

    return true;
}

bool Mercado::atenderEncomenda(Alquimista& jogador, int indexPocao, int indexEncomenda) {
    if (indexPocao < 0 || indexPocao >= static_cast<int>(jogador.colecaoPocoes.size())) {
        std::cout << "  Índice de poção inválido!\n";
        return false;
    }

    if (indexEncomenda < 0 || indexEncomenda >= static_cast<int>(encomendasAtivas.size())) {
        std::cout << "  Índice de encomenda inválido!\n";
        return false;
    }

    const Pocao& pocao = jogador.colecaoPocoes[indexPocao];
    Encomenda& encomenda = encomendasAtivas[indexEncomenda];

    if (encomenda.cumprida) {
        std::cout << "  Esta encomenda já foi cumprida!\n";
        return false;
    }

    // Verificar requisitos mínimos
    bool atendeRequisitos = true;
    for (const auto& [attr, valorMinimo] : encomenda.requisitosMinimos) {
        float valorPocao = 0.0f;
        for (const auto& ing : pocao.receitaUtilizada) {
            auto it = ing.propriedades.find(attr);
            if (it != ing.propriedades.end()) {
                valorPocao += it->second * (ing.ehPocaoReciclada ? 0.8f : 1.0f);
            }
        }

        if (valorPocao < valorMinimo) {
            atendeRequisitos = false;
            std::cout << "  ❌ Atributo " << attributeToString(attr)
                      << " insuficiente (requer " << valorMinimo
                      << ", tem " << valorPocao << ")\n";
            break;
        }
    }

    // Verificar requisitos máximos
    if (atendeRequisitos) {
        for (const auto& [attr, valorMaximo] : encomenda.requisitosMaximos) {
            float valorPocao = 0.0f;
            for (const auto& ing : pocao.receitaUtilizada) {
                auto it = ing.propriedades.find(attr);
                if (it != ing.propriedades.end()) {
                    valorPocao += it->second * (ing.ehPocaoReciclada ? 0.8f : 1.0f);
                }
            }

            if (valorPocao > valorMaximo) {
                atendeRequisitos = false;
                std::cout << "  ❌ Atributo " << attributeToString(attr)
                          << " excede o limite (máx " << valorMaximo
                          << ", tem " << valorPocao << ")\n";
                break;
            }
        }
    }

    if (!atendeRequisitos) {
        std::cout << "  A poção não atende aos requisitos da encomenda.\n";
        return false;
    }

    // Cumprir encomenda
    jogador.ouro += encomenda.recompensaOuro;
    jogador.reputacao += encomenda.recompensaReputacao;

    // Remover a poção do inventário
    jogador.colecaoPocoes.erase(jogador.colecaoPocoes.begin() + indexPocao);

    // Marcar encomenda como cumprida
    encomenda.cumprida = true;

    std::cout << "\n  ✅ ENCOMENDA CUMPRIDA COM SUCESSO!\n";
    std::cout << "  Recompensa: " << Color::YELLOW << encomenda.recompensaOuro
              << " GP" << Color::RESET << " e "
              << Color::GREEN << "+" << encomenda.recompensaReputacao
              << " Reputação" << Color::RESET << "!\n";

    return true;
}

std::string Mercado::requisitosParaString(const Encomenda& encomenda) {
    std::string result = "Requer: ";
    bool first = true;

    for (const auto& [attr, valor] : encomenda.requisitosMinimos) {
        if (!first) result += ", ";
        result += attributeToString(attr) + " ≥ " + std::to_string(static_cast<int>(valor));
        first = false;
    }

    for (const auto& [attr, valor] : encomenda.requisitosMaximos) {
        if (!first) result += ", ";
        result += attributeToString(attr) + " ≤ " + std::to_string(static_cast<int>(valor));
        first = false;
    }

    return result;
}