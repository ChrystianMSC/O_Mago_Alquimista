#include "Alquimista.hpp"

void Alquimista::inicializarInventarioInicial() {
    inventario.clear();

    // 3x Bliss Flower (Heal +5)
    for (int i = 0; i < 3; ++i) {
        inventario.push_back({"ing_1", "Bliss Flower", 1, 10.0f, {{AtributoMagico::HEAL, 5.0f}}});
    }

    // 3x Purified Water (Heal +2, Protection +1)
    for (int i = 0; i < 3; ++i) {
        inventario.push_back({"ing_2", "Purified Water", 1, 5.0f, {{AtributoMagico::HEAL, 2.0f}, {AtributoMagico::PROTECTION, 1.0f}}});
    }

    // 2x Rest Mushroom (Heal +8, Vitality +2)
    for (int i = 0; i < 2; ++i) {
        inventario.push_back({"ing_3", "Rest Mushroom", 1, 15.0f, {{AtributoMagico::HEAL, 8.0f}, {AtributoMagico::VITALITY, 2.0f}}});
    }

    // 2x Small Sunstone (Fire +6)
    for (int i = 0; i < 2; ++i) {
        inventario.push_back({"ing_4", "Small Sunstone", 1, 20.0f, {{AtributoMagico::FIRE, 6.0f}}});
    }
}