
#include "Estrategia.h"
#include <cmath>
#include <algorithm>

Estrategia::Estrategia(int id, bool isTeamA) : id(id), teamA(isTeamA) {
    if (id == 0) role = "Goleiro";
    else if (id == 1) role = "Ala";
    else role = "Atacante";

    lastMoveX = 0.0f;
    lastMoveY = 0.0f;
}

Action Estrategia::think(const GameState& state) {
    Action a{ 0,0 };

    const EntityState& eu = state.getMe();
    const EntityState& bola = state.ball;

    float repulsaoX = 0.0f;
    float repulsaoY = 0.0f;

    float raioDeteccao = 0.15f;
    float intensidadeRepulsao = 0.25f;

    
    auto aplicarRepulsao = [&](const std::vector<EntityState>& robos, float fator) {
        for (const auto& outro : robos) {
            float d = eu.distTo(outro.x, outro.y);
            if (d < raioDeteccao && d > 0.03f) {
                float ang = outro.angleTo(eu.x, eu.y);
                float intensidade = (raioDeteccao - d) / raioDeteccao;

                repulsaoX += std::cos(ang) * intensidade * intensidadeRepulsao * fator;
                repulsaoY += std::sin(ang) * intensidade * intensidadeRepulsao * fator;
            }
        }
        };

    aplicarRepulsao(state.opponents, 1.0f);
    aplicarRepulsao(state.teammates, 1.8f);

    
    if (role == "Goleiro") {
        float minX = teamA ? -0.85f : 0.75f;
        float maxX = teamA ? -0.75f : 0.85f;

        float alvoX = std::clamp(bola.x, std::min(minX, maxX), std::max(minX, maxX));
        float alvoY = std::clamp(bola.y, -0.20f, 0.20f);

        if (eu.distTo(alvoX, alvoY) < 0.02f) {

            float angulo = eu.angleTo(alvoX, alvoY);
            a.moveDirectionX = std::cos(angulo);
            a.moveDirectionY = std::sin(angulo);
        }
        return a;
    }

    
    float minhaDistbola = eu.distTo(bola.x, bola.y);

    float alvoX = bola.x;
    float alvoY = bola.y;
    float velocidade = 1.0f;

    float golInimigoX = teamA ? 0.85f : -0.85f;

    
    if (role == "Atacante") {

        float distancia = minhaDistbola;
        if (teamA) {

            if (distancia > 0.10f) {
                alvoX = bola.x;
                alvoY = bola.y;
                velocidade = 1.2f;
            }
            else {
                alvoX = golInimigoX;
                alvoY = 0.0f;
                velocidade = 1.5f;

                repulsaoX = 0;
                repulsaoY = 0;
            }
        }
        else {
            if (distancia > 0.10f) {
                alvoX = bola.x;
                alvoY = bola.y;
                velocidade = 1.2f;
            }
            else {
                alvoX = golInimigoX;
                alvoY = 0.0f;
                velocidade = 1.5f;

                repulsaoX = 0;
                repulsaoY = 0;
            }
        }
    }
    
    
    if (role == "Ala") {
        if (teamA)
            alvoX = std::clamp(bola.x - 0.20f, -0.70f, -0.40f);
        else
            alvoX = std::clamp(bola.x + 0.20f, 0.40f, 0.70f);

        alvoY = std::clamp(bola.y, -0.50f, 0.50f);
    }

    //evit tremor
    float distAlvo = eu.distTo(alvoX, alvoY);
    if (distAlvo < 0.05f) return a;
    float angulo = eu.angleTo(alvoX, alvoY);

    float finalX = std::cos(angulo);
    float finalY = std::sin(angulo);

    // reduz repulsão perto da bola
    float fatorRepulsao = std::clamp(distAlvo / 0.1f, 0.0f, 1.0f);

    finalX += repulsaoX * fatorRepulsao;
    finalY += repulsaoY * fatorRepulsao;

    // empurrão lateral (quebra simetria)
    float empurrao = teamA ?  0.1f : -0.1f;
    finalY += empurrao;

    float vetorresultante = std::sqrt(finalX * finalX + finalY * finalY);

    if (vetorresultante < 0.2f) {
        finalX = std::cos(angulo + 1.57f);
        finalY = std::sin(angulo + 1.57f);
        vetorresultante = 1.0f;
    }

    finalX = (finalX / vetorresultante) * velocidade;
    finalY = (finalY / vetorresultante) * velocidade;

    //Impede mudança brusca de direção (tremor)
    float smooth = 0.8f;
    //Direção atual: smooth *LastMoveX, Direção futura amenizada: (1-smooth) * finalX
    a.moveDirectionX = smooth * lastMoveX + (1 - smooth) * finalX;
    a.moveDirectionY = smooth * lastMoveY + (1 - smooth) * finalY;

    lastMoveX = a.moveDirectionX;
    lastMoveY = a.moveDirectionY;

    return a;
}
    
    
 
