#ifndef GHOST_H
#define GHOST_H

#include "board.h"
#include <curses.h>

enum class GhostState {
    NORMAL,         // Estado normal - perseguindo o Pacman
    VULNERABLE,     // Estado vulnerável - quando Pacman pega power pellet
    RETURNING,      // Estado retornando - quando foi comido
    WAITING         // Estado esperando - no spawn
};

enum class GhostType {
    BLINKY,  // Vermelho - persegue diretamente
    PINKY,   // Rosa - tenta prever movimento
    INKY,    // Azul - comportamento mais errático
    CLYDE    // Laranja - alterna entre perseguir e fugir
};

class Ghost {
private:
    int x, y;                  // Posição atual
    int spawnX, spawnY;        // Posição inicial/respawn
    int speed;                 // Velocidade de movimento
    GhostState state;          // Estado atual
    GhostType type;           // Tipo do fantasma
    int vulnerableTimer;       // Tempo restante de vulnerabilidade
    bool isActive;             // Se está em jogo
    chtype ghostChar;          // Caractere para desenhar
    int colorPair;             // Par de cores do PDCurses

public:
    // Construtor
    Ghost(int startX, int startY, GhostType ghostType);

    // Movimentação
    void move(int pacmanX, int pacmanY, Board& board);
    void returnToSpawn();

    // Estados
    void makeVulnerable();
    void recover();
    bool isVulnerable() const;

    // Respawn
    void respawn();

    // Visualização
    void draw();
    void updateDisplay();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    GhostState getState() const { return state; }
    bool getIsActive() const { return isActive; }

    // Setters
    void setState(GhostState newState);
    void setPosition(int newX, int newY);

private:
    //  movimento 
    void moveNormal(int pacmanX, int pacmanY, Board& board);
    void moveVulnerable(int pacmanX, int pacmanY, Board& board);
    void moveReturning(Board& board);

    void moveBlinky(int pacmanX, int pacmanY, Board& board);
    void movePinky(int pacmanX, int pacmanY, Board& board);
    void moveInky(int pacmanX, int pacmanY, Board& board);
    void moveClyde(int pacmanX, int pacmanY, Board& board);

    bool canMoveTo(int newX, int newY, Board& board);
    void calculateNextMove(int targetX, int targetY, Board& board, int& nextX, int& nextY);
};

#endif