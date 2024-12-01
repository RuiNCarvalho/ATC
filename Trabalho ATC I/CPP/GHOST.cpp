#include "ghost.h"
#include <curses.h>  // Inclus�o da biblioteca PDCurses

Ghost::Ghost(int startX, int startY)
    : GameObject(startX, startY, 1, 1, 1),  // Adaptado para caractere �nico no console
    currentState(GhostState::SCATTER),
    personality(nullptr) {
    // Inicializa��o espec�fica para PDCurses
}

void Ghost::move() {
    // Movimento adaptado para PDCurses
    switch (currentState) {
    case GhostState::CHASE:
        // Movimento de persegui��o usando coordenadas do console
        x += (rand() % 3 - 1);  // movimento aleat�rio horizontal
        y += (rand() % 3 - 1);  // movimento aleat�rio vertical
        break;

    case GhostState::SCATTER:
        // Movimento mais err�tico
        x += (rand() % 5 - 2);
        y += (rand() % 5 - 2);
        break;

    case GhostState::FRIGHTENED:
        // Movimento completamente aleat�rio
        x += (rand() % 5 - 2);
        y += (rand() % 5 - 2);
        break;

    case GhostState::RETURNING:
        // Movimento de retorno para posi��o inicial
        // Implementa��o simples de retorno
        x = (x > startX) ? x - 1 :
            (x < startX) ? x + 1 : x;
        y = (y > startY) ? y - 1 :
            (y < startY) ? y + 1 : y;
        break;
    }

    // Limites da tela 
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    x = (x < 0) ? 0 : (x >= max_x) ? max_x - 1 : x;
    y = (y < 0) ? 0 : (y >= max_y) ? max_y - 1 : y;
}

void Ghost::draw() {
    // Desenho espec�fico para PDCurses
    switch (currentState) {
    case GhostState::CHASE:
        mvaddch(y, x, 'G');  // 'G' de Ghost
        break;
    case GhostState::SCATTER:
        mvaddch(y, x, 'g');  // Letra min�scula para scatter
        break;
    case GhostState::FRIGHTENED:
        mvaddch(y, x, '?');  // Simbolo de d�vida quando assustado
        break;
    case GhostState::RETURNING:
        mvaddch(y, x, 'H');  // 'H' de Home (retornando)
        break;
    }
}

// M�todos adicionais para PDCurses
void Ghost::handleCollision(Pacman* pacman) {
    // L�gica de colis�o usando coordenadas do console
    if (x == pacman->getX() && y == pacman->getY()) {
        switch (currentState) {
        case GhostState::CHASE:
            pacman->loseLife();
            break;
        case GhostState::FRIGHTENED:
            // Ghost pode ser "comido"
            resetPosition();
            break;
        }
    }
}
Last edited just now