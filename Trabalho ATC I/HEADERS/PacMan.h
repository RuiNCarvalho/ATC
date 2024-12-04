#ifndef PACMAN_H
#define PACMAN_H

#include "game_object.h"
#include "board.h"
#include <curses.h>

class Pacman : public GameObject {
private:
    // Direção atual do movimento
    int direction_x;  // -1 (esquerda), 0 (parado) ou 1 (direita)
    int direction_y;  // -1 (cima), 0 (parado) ou 1 (baixo)

    // Posição do spawn (onde o Pacman começa/renasce)
    int spawn_x;
    int spawn_y;

    // Status do jogador
    int lives;        // Número de vidas
    int score;        // Pontuação
    bool isPowered;   // Se está com power pellet ativo
    int powerTimer;   // Tempo restante do power pellet
    

    // Caractere para desenhar o Pacman
    chtype pacmanChar;

public:
    // Construtor - cria o Pacman na posição inicial
    Pacman(int startX, int startY);

    // Movimento e controle
    void move(Board& board);
    void changeDirection(int dx, int dy);
    bool canwalk(int newX, int newY, const Board& board);

    // Ações do jogador
    void collectPellet(int points);    // Coleta pellet normal
    void collectPowerPellet();         // Coleta power pellet
    void loseLife();                   // Perde uma vida
    void respawn();                    // Volta ao ponto inicial

    // Status e poderes
    bool isPowerPelletActive() const;  // Verifica se está com poder
    void updatePowerState();           // Atualiza estado do poder

    // Visualização
    void draw();                       // Desenha o Pacman

    // Getters - funções para obter informações
    int getLives() const;
    int getScore() const;
    int getDirectionX() const;
    int getDirectionY() const;
};

#endif 

