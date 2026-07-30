// src/main.cpp
#include <iostream>
#include <vector>
#include <limits>
#include <random>  // Adicionar este include
#include "Tipos.hpp"
#include "Ingrediente.hpp"
#include "Pocao.hpp"
#include "Caldeirao.hpp"
#include "Alquimista.hpp"
#include "GeradorProcedural.hpp"
#include "Mercado.hpp"

#ifdef _WIN32
#include <windows.h>
void setupWindowsTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
}
#else
void setupWindowsTerminal() {}
#endif

void displayHeader(const Alquimista& player) {
    std::cout << "\n" << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   ✧ O MAGO ALQUIMISTA ✧                        ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;
    std::cout << "  💰 Ouro: " << Color::YELLOW << player.ouro << " GP" << Color::RESET
              << "  ⭐ Reputação: " << player.reputacao
              << "  📦 Inventário: " << player.inventario.size() << " itens"
              << "  🧪 Poções: " << player.colecaoPocoes.size() << "\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n";
}

void displayMenu() {
    std::cout << "\n" << Color::BOLD << "  MENU PRINCIPAL" << Color::RESET << "\n";
    std::cout << "  ────────────────────────────────────\n";
    std::cout << "  1.  📋 Ver Inventário\n";
    std::cout << "  2.  🧪 Ir ao Caldeirão (Criar Poção)\n";
    std::cout << "  3.  🏪 Visitar Mercado\n";
    std::cout << "  4.  📜 Ver Encomendas\n";
    std::cout << "  5.  💼 Ver Coleção de Poções\n";
    std::cout << "  6.  💰 Vender Poção\n";
    std::cout << "  0.  🚪 Sair do Jogo\n";
    std::cout << "  ────────────────────────────────────\n";
    std::cout << "  Escolha uma opção: ";
}

int getValidatedInput(int min, int max) {
    int input;
    while (!(std::cin >> input) || input < min || input > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Entrada inválida. Tente novamente: ";
    }
    return input;
}

int main() {
    setupWindowsTerminal();

    // Inicializar jogo
    Alquimista player;
    player.inicializarInventarioInicial();

    Mercado mercado;
    mercado.iniciarNovoDia(player.reputacao);

    GeradorProcedural gerador;

    // Criar um gerador de números aleatórios para eventos
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> eventoDist(0, 100);

    bool running = true;
    int diaAtual = 1;

    std::cout << Color::BOLD << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         🌟 BEM-VINDO, GRANDE ALQUIMISTA! 🌟                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;
    std::cout << "  O mundo aguarda suas poções mágicas!\n";
    std::cout << "  Dia " << diaAtual << " do seu aprendizado alquímico.\n\n";

    while (running) {
        displayHeader(player);
        displayMenu();

        int option = getValidatedInput(0, 6);
        std::cout << "\n";

        switch (option) {
            case 1: { // Ver Inventário
                std::cout << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
                std::cout << "║                    📋 INVENTÁRIO DO MAGO                        ║\n";
                std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;

                if (player.inventario.empty()) {
                    std::cout << "  Seu inventário está vazio! Visite o mercado para comprar ingredientes.\n";
                } else {
                    for (size_t i = 0; i < player.inventario.size(); ++i) {
                        const auto& ing = player.inventario[i];

                        std::string corRaridade;
                        if (ing.raridade <= 2) corRaridade = Color::GRAY;
                        else if (ing.raridade <= 3) corRaridade = Color::BLUE;
                        else if (ing.raridade <= 4) corRaridade = Color::PURPLE;
                        else corRaridade = Color::YELLOW;

                        std::cout << "  " << i + 1 << ". "
                                  << corRaridade << ing.nome << Color::RESET
                                  << " [R" << ing.raridade << "]";

                        if (ing.ehPocaoReciclada) {
                            std::cout << " ♻️ (reciclada)";
                        }

                        std::cout << "\n     Atributos: ";
                        bool first = true;
                        for (const auto& [attr, val] : ing.propriedades) {
                            if (!first) std::cout << ", ";
                            std::cout << attributeToString(attr) << " +" << val;
                            first = false;
                        }
                        std::cout << "\n";
                    }
                }
                break;
            }

            case 2: { // Caldeirão
                if (player.inventario.size() < 2) {
                    std::cout << Color::RED << "❌ Você precisa de pelo menos 2 ingredientes no inventário.\n" << Color::RESET;
                    break;
                }

                std::cout << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
                std::cout << "║                    🧪 CALDEIRÃO MÁGICO                         ║\n";
                std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;
                std::cout << "  Selecione 2 ingredientes para misturar:\n\n";

                for (size_t i = 0; i < player.inventario.size(); ++i) {
                    const auto& ing = player.inventario[i];
                    std::cout << "  " << i + 1 << ". " << ing.nome;
                    if (ing.ehPocaoReciclada) std::cout << " ♻️";
                    std::cout << "\n";
                }

                std::cout << "\n  Escolha o 1º ingrediente: ";
                int idx1 = getValidatedInput(1, player.inventario.size()) - 1;

                std::cout << "  Escolha o 2º ingrediente: ";
                int idx2 = getValidatedInput(1, player.inventario.size()) - 1;

                if (idx1 == idx2) {
                    std::cout << Color::RED << "❌ Você precisa escolher ingredientes diferentes!\n" << Color::RESET;
                    break;
                }

                std::vector<Ingrediente> mixture = {
                    player.inventario[idx1],
                    player.inventario[idx2]
                };

                Pocao newPotion = Caldeirao::misturar(mixture);

                std::cout << "\n✨ " << Color::BOLD << "POÇÃO CRIADA!" << Color::RESET << " ✨\n";
                std::cout << "  Nome: " << Color::BOLD << newPotion.nome << Color::RESET << "\n";
                std::cout << "  Efeito: " << attributeToString(newPotion.efeitoDominante) << "\n";
                std::cout << "  Potência: " << newPotion.potencia << "\n";
                std::cout << "  Pureza: " << (newPotion.pureza * 100.0f) << "%\n";
                std::cout << "  💰 Valor: " << Color::YELLOW << newPotion.valorComercial << " GP" << Color::RESET << "\n";

                // Remover ingredientes usados
                if (idx1 > idx2) {
                    player.inventario.erase(player.inventario.begin() + idx1);
                    player.inventario.erase(player.inventario.begin() + idx2);
                } else {
                    player.inventario.erase(player.inventario.begin() + idx2);
                    player.inventario.erase(player.inventario.begin() + idx1);
                }

                player.colecaoPocoes.push_back(newPotion);
                break;
            }

            case 3: { // Mercado
                std::cout << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
                std::cout << "║                   🏪 MERCADO MÁGICO                             ║\n";
                std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;

                std::cout << "  Digite o número do ingrediente para comprar, ou 0 para voltar.\n\n";
                mercado.exibirEstoque();

                std::cout << "  Escolha: ";
                int escolha = getValidatedInput(0, mercado.getEstoque().size());

                if (escolha > 0) {
                    mercado.comprarIngrediente(player, escolha - 1);
                }
                break;
            }

            case 4: { // Ver Encomendas
                mercado.exibirEncomendas();

                if (!mercado.getEncomendas().empty()) {
                    std::cout << "  Deseja tentar cumprir alguma encomenda? (s/n): ";
                    char resposta;
                    std::cin >> resposta;

                    if (resposta == 's' || resposta == 'S') {
                        if (player.colecaoPocoes.empty()) {
                            std::cout << "  Você não tem poções para oferecer.\n";
                            break;
                        }

                        std::cout << "\n  Selecione a poção (1-" << player.colecaoPocoes.size() << "): ";
                        int idxPocao = getValidatedInput(1, player.colecaoPocoes.size()) - 1;

                        std::cout << "  Selecione a encomenda (1-" << mercado.getEncomendas().size() << "): ";
                        int idxEnc = getValidatedInput(1, mercado.getEncomendas().size()) - 1;

                        mercado.atenderEncomenda(player, idxPocao, idxEnc);
                    }
                }
                break;
            }

            case 5: { // Ver Coleção de Poções
                std::cout << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
                std::cout << "║                    💼 COLEÇÃO DE POÇÕES                       ║\n";
                std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;

                if (player.colecaoPocoes.empty()) {
                    std::cout << "  Você ainda não criou nenhuma poção.\n";
                } else {
                    for (size_t i = 0; i < player.colecaoPocoes.size(); ++i) {
                        const auto& p = player.colecaoPocoes[i];
                        std::cout << "  " << i + 1 << ". " << Color::BOLD << p.nome << Color::RESET
                                  << "\n     Efeito: " << attributeToString(p.efeitoDominante)
                                  << " | Potência: " << p.potencia
                                  << " | Pureza: " << (p.pureza * 100.0f) << "%"
                                  << " | Valor: " << Color::YELLOW << p.valorComercial << " GP" << Color::RESET << "\n";
                    }
                }
                break;
            }

            case 6: { // Vender Poção
                std::cout << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
                std::cout << "║                    💰 VENDER POÇÃO                           ║\n";
                std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;

                if (player.colecaoPocoes.empty()) {
                    std::cout << "  Você não tem poções para vender.\n";
                    break;
                }

                for (size_t i = 0; i < player.colecaoPocoes.size(); ++i) {
                    const auto& p = player.colecaoPocoes[i];
                    std::cout << "  " << i + 1 << ". " << p.nome
                              << " (Valor: " << Color::YELLOW << p.valorComercial * 0.7f << " GP" << Color::RESET << ")\n";
                }

                std::cout << "\n  Escolha a poção para vender (0 para voltar): ";
                int escolha = getValidatedInput(0, player.colecaoPocoes.size());

                if (escolha > 0) {
                    mercado.venderPocao(player, escolha - 1);
                }
                break;
            }

            case 0: { // Sair
                std::cout << "  Tem certeza que deseja sair? (s/n): ";
                char confirm;
                std::cin >> confirm;
                if (confirm == 's' || confirm == 'S') {
                    running = false;
                }
                break;
            }

            default:
                std::cout << Color::RED << "  Opção inválida!\n" << Color::RESET;
        }

        // Pausa para leitura
        if (running && option != 0) {
            std::cout << "\n  Pressione Enter para continuar...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }

        // Passar dia automaticamente após algumas ações
        if (option == 3 || option == 4 || option == 6) {
            std::cout << "\n  🌅 Um novo dia começa no mercado...\n";
            diaAtual++;
            mercado.iniciarNovoDia(player.reputacao);

            // Oportunidade para eventos aleatórios
            if (eventoDist(rng) < 20) {  // CORRIGIDO: Usando o gerador persistente
                std::cout << Color::YELLOW << "  ✨ Um viajante misterioso oferece um ingrediente raro!\n" << Color::RESET;
                Ingrediente ingRaro = gerador.gerarIngrediente(player.reputacao + 3);
                if (player.ouro >= ingRaro.precoBase * 0.8f) {
                    std::cout << "  Você pode comprar " << ingRaro.nome
                              << " por " << Color::YELLOW << ingRaro.precoBase * 0.8f << " GP" << Color::RESET
                              << " (deseja comprar? s/n): ";
                    char comprar;
                    std::cin >> comprar;
                    if (comprar == 's' || comprar == 'S') {
                        player.ouro -= ingRaro.precoBase * 0.8f;
                        player.inventario.push_back(ingRaro);
                        std::cout << Color::GREEN << "  Compra realizada!\n" << Color::RESET;
                    }
                }
            }
        }
    }

    std::cout << "\n" << Color::BOLD << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          ✨ OBRIGADO POR JOGAR O MAGO ALQUIMISTA! ✨               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n" << Color::RESET;
    std::cout << "  Poções criadas: " << player.colecaoPocoes.size()
              << " | Reputação final: " << player.reputacao
              << " | Ouro final: " << player.ouro << " GP\n\n";

    return 0;
}