#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

#include "Interfaces.h"
#include <string>

class Estrategia : public PlayerAgent {
public:
    Estrategia(int id, bool isTeamA);
    
    // Ponto de entrada da lógica do candidato
    Action think(const GameState& state) override;

private:
    int id;
    bool teamA;
    std::string role; // "Goleiro", "Ala", "Atacante"
    
    float lastMoveX = 0.0f;
    float lastMoveY = 0.0f; // Para evitar tremores
    // Você é livre para criar variáveis, instanciar Máquinas de Estado ou Behavior Trees aqui.
};

#endif // ESTRATEGIA_H
