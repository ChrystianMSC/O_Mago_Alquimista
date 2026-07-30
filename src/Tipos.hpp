// src/Tipos.hpp
#pragma once
#include <string>
#include <chrono>

enum class AtributoMagico {
    HEAL,
    VITALITY,
    FIRE,
    ICE,
    POISON,
    PROTECTION,
    LEVITATION,
    MYSTERY
};

// ANSI Color Codes
namespace Color {
    inline const std::string RESET   = "\033[0m";
    inline const std::string GREEN   = "\033[32m"; // Heal
    inline const std::string RED     = "\033[31m"; // Vitality
    inline const std::string ORANGE  = "\033[33m"; // Fire
    inline const std::string BLUE    = "\033[36m"; // Ice
    inline const std::string PURPLE  = "\033[35m"; // Poison
    inline const std::string GRAY    = "\033[37m"; // Protection
    inline const std::string PINK    = "\033[95m"; // Levitation
    inline const std::string YELLOW  = "\033[93m"; // Mystery / Gold
    inline const std::string BOLD    = "\033[1m";
}

// Convert enum to formatted text
inline std::string attributeToString(AtributoMagico attr) {
    switch (attr) {
    case AtributoMagico::HEAL:       return Color::GREEN + "Cura" + Color::RESET;
    case AtributoMagico::VITALITY:   return Color::RED + "Vitalidade" + Color::RESET;
    case AtributoMagico::FIRE:       return Color::ORANGE + "Fogo" + Color::RESET;
    case AtributoMagico::ICE:        return Color::BLUE + "Gelo" + Color::RESET;
    case AtributoMagico::POISON:     return Color::PURPLE + "Veneno" + Color::RESET;
    case AtributoMagico::PROTECTION: return Color::GRAY + "Proteção" + Color::RESET;
    case AtributoMagico::LEVITATION: return Color::PINK + "Levitação" + Color::RESET;
    case AtributoMagico::MYSTERY:    return Color::YELLOW + "Mistério" + Color::RESET;
    }
    return "Desconhecido";
}