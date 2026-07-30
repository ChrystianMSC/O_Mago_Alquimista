// src/GeradorProcedural.cpp
#include "GeradorProcedural.hpp"
#include <algorithm>
#include <numeric>

GeradorProcedural::GeradorProcedural()
    : rng(std::random_device{}()) {
    // Inicialização do gerador com seed aleatória
}

std::string GeradorProcedural::getPrefixPorRaridade(int raridade) {
    std::uniform_int_distribution<int> dist;

    if (raridade <= 2) {
        // Comum/Incomum
        dist = std::uniform_int_distribution<int>(0, prefixosComuns.size() - 1);
        return prefixosComuns[dist(rng)];
    } else if (raridade <= 4) {
        // Raro/Épico
        dist = std::uniform_int_distribution<int>(0, prefixosRaros.size() - 1);
        return prefixosRaros[dist(rng)];
    } else {
        // Lendário
        dist = std::uniform_int_distribution<int>(0, prefixosLendarios.size() - 1);
        return prefixosLendarios[dist(rng)];
    }
}

AtributoMagico GeradorProcedural::getAtributoAleatorio() {
    std::uniform_int_distribution<int> dist(0, 7);
    return static_cast<AtributoMagico>(dist(rng));
}

float GeradorProcedural::gerarValorAtributo(int raridade, bool primario) {
    float base = multiplicadorRaridade[raridade];
    float variacao = distFloat(rng) * base * 0.3f; // 30% de variação

    if (primario) {
        // Atributo primário é mais forte
        return base * 5.0f + variacao;
    } else {
        // Atributo secundário é mais fraco
        return base * 2.0f + variacao;
    }
}

std::unordered_map<AtributoMagico, float> GeradorProcedural::gerarPropriedades(int raridade) {
    std::unordered_map<AtributoMagico, float> propriedades;

    // Número de atributos baseado na raridade
    int numAtributosPrimarios;
    int numAtributosSecundarios;

    switch(raridade) {
        case 1: // Comum
            numAtributosPrimarios = 1;
            numAtributosSecundarios = 0;
            break;
        case 2: // Incomum
            numAtributosPrimarios = 1;
            numAtributosSecundarios = 1;
            break;
        case 3: // Raro
            numAtributosPrimarios = 2;
            numAtributosSecundarios = 1;
            break;
        case 4: // Épico
            numAtributosPrimarios = 2;
            numAtributosSecundarios = 2;
            break;
        case 5: // Lendário
            numAtributosPrimarios = 3;
            numAtributosSecundarios = 2;
            break;
        default:
            numAtributosPrimarios = 1;
            numAtributosSecundarios = 0;
    }

    // Gerar atributos primários (fortes)
    std::vector<AtributoMagico> atributosUsados;
    for (int i = 0; i < numAtributosPrimarios; ++i) {
        AtributoMagico attr;
        do {
            attr = getAtributoAleatorio();
        } while (std::find(atributosUsados.begin(), atributosUsados.end(), attr) != atributosUsados.end());

        atributosUsados.push_back(attr);
        propriedades[attr] = gerarValorAtributo(raridade, true);
    }

    // Gerar atributos secundários (mais fracos)
    for (int i = 0; i < numAtributosSecundarios; ++i) {
        AtributoMagico attr;
        do {
            attr = getAtributoAleatorio();
        } while (std::find(atributosUsados.begin(), atributosUsados.end(), attr) != atributosUsados.end());

        atributosUsados.push_back(attr);
        propriedades[attr] = gerarValorAtributo(raridade, false);
    }

    return propriedades;
}

std::string GeradorProcedural::gerarNomeIngrediente(int raridade) {
    std::string nome;

    // 20% de chance de ter sufixo (mais raro = mais chance)
    float chanceSufixo = 0.2f + (raridade - 1) * 0.1f;
    bool temSufixo = distFloat(rng) < chanceSufixo;

    // Escolher prefixo baseado na raridade
    std::string prefixo = getPrefixPorRaridade(raridade);

    // Escolher substantivo
    std::uniform_int_distribution<int> distSub(0, substantivos.size() - 1);
    std::string substantivo = substantivos[distSub(rng)];

    // Montar nome
    nome = prefixo + " " + substantivo;

    if (temSufixo) {
        std::uniform_int_distribution<int> distSuf(0, sufixos.size() - 1);
        nome += " " + sufixos[distSuf(rng)];
    }

    return nome;
}

float GeradorProcedural::calcularPrecoBase(
    int raridade,
    const std::unordered_map<AtributoMagico, float>& propriedades) {

    // Soma total de todos os atributos
    float totalAtributos = 0.0f;
    for (const auto& [attr, valor] : propriedades) {
        totalAtributos += valor;
    }

    // Preço base = (soma atributos * 1.5) * multiplicador de raridade
    float preco = totalAtributos * 1.5f * multiplicadorRaridade[raridade];

    // Adicionar um fator aleatório (+/- 20%)
    preco *= (0.8f + distFloat(rng) * 0.4f);

    // Arredondar para 2 casas decimais
    preco = std::round(preco * 100.0f) / 100.0f;

    return std::max(5.0f, preco); // Preço mínimo de 5
}

Ingrediente GeradorProcedural::gerarIngrediente(int nivelJogador) {
    // Calcular raridade baseada no nível do jogador
    // Nível 1-5: chance maior de itens comuns
    // Nível 6-10: chances balanceadas
    // Nível 11+: chance maior de itens raros

    int raridade;
    std::uniform_int_distribution<int> distRaridade(1, 100);
    int chance = distRaridade(rng);

    if (nivelJogador <= 5) {
        // Iniciante: maioria comum
        if (chance <= 60) raridade = 1;
        else if (chance <= 85) raridade = 2;
        else if (chance <= 95) raridade = 3;
        else if (chance <= 98) raridade = 4;
        else raridade = 5;
    } else if (nivelJogador <= 10) {
        // Intermediário: balanceado
        if (chance <= 30) raridade = 1;
        else if (chance <= 60) raridade = 2;
        else if (chance <= 80) raridade = 3;
        else if (chance <= 93) raridade = 4;
        else raridade = 5;
    } else {
        // Avançado: mais itens raros
        if (chance <= 15) raridade = 1;
        else if (chance <= 35) raridade = 2;
        else if (chance <= 60) raridade = 3;
        else if (chance <= 85) raridade = 4;
        else raridade = 5;
    }

    return gerarIngredienteComRaridade(raridade);
}

Ingrediente GeradorProcedural::gerarIngredienteComRaridade(int raridade) {
    Ingrediente ing;

    // Validar raridade
    raridade = std::max(1, std::min(5, raridade));
    ing.raridade = raridade;

    // Gerar nome
    ing.nome = gerarNomeIngrediente(raridade);

    // Gerar propriedades
    ing.propriedades = gerarPropriedades(raridade);

    // Calcular preço base
    ing.precoBase = calcularPrecoBase(raridade, ing.propriedades);

    // Gerar ID único (timestamp + random)
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    ing.id = "proc_" + std::to_string(timestamp) + "_" + std::to_string(distInt(rng));

    // Garantir que não é uma poção reciclada
    ing.ehPocaoReciclada = false;

    return ing;
}

Ingrediente GeradorProcedural::gerarIngredienteComAtributo(AtributoMagico atributo, int raridade) {
    Ingrediente ing = gerarIngredienteComRaridade(raridade);

    // Forçar o atributo desejado como primário
    // Sobrescrever o atributo existente ou adicionar
    float valorPrimario = gerarValorAtributo(raridade, true);
    ing.propriedades[atributo] = valorPrimario;

    // Recalcular preço
    ing.precoBase = calcularPrecoBase(raridade, ing.propriedades);

    return ing;
}

std::vector<Ingrediente> GeradorProcedural::gerarLoteDeIngredientes(
    int nivelJogador,
    int quantidade) {

    std::vector<Ingrediente> lote;
    lote.reserve(quantidade);

    for (int i = 0; i < quantidade; ++i) {
        lote.push_back(gerarIngrediente(nivelJogador));
    }

    return lote;
}