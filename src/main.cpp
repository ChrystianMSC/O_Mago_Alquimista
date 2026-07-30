#include <iostream>
#include <vector>
#include "Tipos.hpp"
#include "Ingrediente.hpp"
#include "Pocao.hpp"
#include "Caldeirao.hpp"
#include "Alquimista.hpp"

#ifdef _WIN32
#include <windows.h>
// Enable ANSI Color codes and UTF-8 output on Windows Terminal/CMD
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
    std::cout << "\n==================================================\n";
    std::cout << Color::BOLD << "             THE ALCHEMIST MAGE V1.0" << Color::RESET << "\n";
    std::cout << " Gold: " << Color::YELLOW << player.ouro << " GP" << Color::RESET
              << " | Reputation: " << player.reputacao << "\n";
    std::cout << "==================================================\n";
}

int main() {
    setupWindowsTerminal();

    Alquimista player;
    player.inicializarInventarioInicial();

    bool running = true;
    while (running) {
        displayHeader(player);
        std::cout << "1. View Ingredient Inventory\n";
        std::cout << "2. Go to Cauldron (Brew Potion)\n";
        std::cout << "3. View Potion Collection\n";
        std::cout << "0. Exit Game\n";
        std::cout << "Choose an action: ";

        int option;
        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (option) {
            case 1: {
                std::cout << "\n--- INVENTORY ---\n";
                for (size_t i = 0; i < player.inventario.size(); ++i) {
                    const auto& ing = player.inventario[i];
                    std::cout << i + 1 << ". " << ing.nome << " (Properties: ";
                    for (const auto& [attr, val] : ing.propriedades) {
                        std::cout << attributeToString(attr) << " +" << val << " ";
                    }
                    std::cout << ")\n";
                }
                break;
            }
            case 2: {
                if (player.inventario.size() < 2) {
                    std::cout << "\n[!] You need at least 2 ingredients in your inventory.\n";
                    break;
                }

                std::cout << "\n--- SELECT 2 INGREDIENTS ---\n";
                for (size_t i = 0; i < player.inventario.size(); ++i) {
                    std::cout << i + 1 << ". " << player.inventario[i].nome << "\n";
                }

                int idx1, idx2;
                std::cout << "Choose 1st ingredient (number): ";
                std::cin >> idx1;
                std::cout << "Choose 2nd ingredient (number): ";
                std::cin >> idx2;

                if (idx1 > 0 && idx1 <= (int)player.inventario.size() &&
                    idx2 > 0 && idx2 <= (int)player.inventario.size() && idx1 != idx2) {

                    int i1 = idx1 - 1;
                    int i2 = idx2 - 1;

                    std::vector<Ingrediente> mixture = {
                        player.inventario[i1],
                        player.inventario[i2]
                    };

                    Pocao newPotion = Caldeirao::misturar(mixture);

                    std::cout << "\n✨ " << Color::BOLD << "POTION CREATED!" << Color::RESET << " ✨\n";
                    std::cout << "Name: " << newPotion.nome << "\n";
                    std::cout << "Primary Effect: " << attributeToString(newPotion.efeitoDominante) << "\n";
                    std::cout << "Potency: " << newPotion.potencia << "\n";
                    std::cout << "Purity: " << (newPotion.pureza * 100.0f) << "%\n";
                    std::cout << "Market Value: " << Color::YELLOW << newPotion.valorComercial << " GP" << Color::RESET << "\n";

                    if (i1 > i2) {
                        player.inventario.erase(player.inventario.begin() + i1);
                        player.inventario.erase(player.inventario.begin() + i2);
                    } else {
                        player.inventario.erase(player.inventario.begin() + i2);
                        player.inventario.erase(player.inventario.begin() + i1);
                    }

                    player.colecaoPocoes.push_back(newPotion);
                } else {
                    std::cout << "\n[!] Invalid selection or repeated ingredients!\n";
                }
                break;
            }
            case 3: {
                std::cout << "\n--- POTION COLLECTION ---\n";
                if (player.colecaoPocoes.empty()) {
                    std::cout << "No potions created yet.\n";
                } else {
                    for (size_t i = 0; i < player.colecaoPocoes.size(); ++i) {
                        const auto& p = player.colecaoPocoes[i];
                        std::cout << i + 1 << ". " << p.nome << " | Potency: " << p.potencia
                                  << " | Purity: " << (p.pureza * 100.0f) << "% | Value: "
                                  << Color::YELLOW << p.valorComercial << " GP" << Color::RESET << "\n";
                    }
                }
                break;
            }
            case 0:
                running = false;
                break;
            default:
                std::cout << "\n[!] Invalid option.\n";
        }
    }

    std::cout << "\nThank you for playing The Alchemist Mage!\n";
    return 0;
}