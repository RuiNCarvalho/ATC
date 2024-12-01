#ifndef PACMAN_UI_H
#define PACMAN_UI_H

#include "pacman.h"
#include "ghost.h"
   
//esta class vai desenhar todos os elemnetos que irao aparecer na tela do jogo

class PacmanUI {
public:
   
    static void drawPacman(const Pacman& pacman);
    static void drawGhost(const Ghost& ghost);
    static void drawmaze(const char* maze[], int height);
    static void drawScore(const Pacman& pacman);
    static void drawGameOver(int finalScore);
    static void drawTitle();
};

#endif