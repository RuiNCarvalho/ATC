#include "ghost.h"
#include "pacman_ui.h"
#include <cstdlib>
#include <cmath>

Ghost::Ghost(int startX, int startY, GhostType ghostType)
    : x(startX), y(startY), spawnX(startX), spawnY(startY),
    speed(1), state(GhostState::WAITING), type(ghostType),
    vulnerableTimer(0), isActive(true) {

    // Define aparência baseada no tipo
    switch (type) {
    case GhostType::BLINKY:
        colorPair = 2;  // Vermelho (definido na PacmanUI)
        ghostChar = 'B';
        break;
    case GhostType::PINKY:
        colorPair = 3;  // Magenta (definido na PacmanUI)
        ghostChar = 'P';
        break;
    case GhostType::INKY:
        colorPair = 4;  // Cyan (definido na PacmanUI)
        ghostChar = 'I';
        break;
    case GhostType::CLYDE:
        colorPair = 5;  // Verde (definido na PacmanUI)
        ghostChar = 'C';
        break;
    }
}

void Ghost::move(int pacmanX, int pacmanY, Board& board) {
    if (!isActive) return;

    if (state == GhostState::VULNERABLE) {
        if (--vulnerableTimer <= 0) {
            recover();
        }
    }

    switch (state) {
    case GhostState::NORMAL:
        moveNormal(pacmanX, pacmanY, board);
        break;
    case GhostState::VULNERABLE:
        moveVulnerable(pacmanX, pacmanY, board);
        break;
    case GhostState::RETURNING:
        moveReturning(board);
        break;
    case GhostState::WAITING:
        break;
    }
}

void Ghost::moveNormal(int pacmanX, int pacmanY, Board& board) {
    switch (type) {
    case GhostType::BLINKY:
        moveBlinky(pacmanX, pacmanY, board);
        break;
    case GhostType::PINKY:
        movePinky(pacmanX, pacmanY, board);
        break;
    case GhostType::INKY:
        moveInky(pacmanX, pacmanY, board);
        break;
    case GhostType::CLYDE:
        moveClyde(pacmanX, pacmanY, board);
        break;
    }
}

void Ghost::moveVulnerable(int pacmanX, int pacmanY, Board& board) {
    // Movimento aleatório quando vulnerável
    int randDir = rand() % 4;
    int newX = x, newY = y;
    switch (randDir) {
    case 0: newX++; break;
    case 1: newX--; break;
    case 2: newY++; break;
    case 3: newY--; break;
    }
    if (canMoveTo(newX, newY, board)) {
        x = newX;
        y = newY;
    }
}

void Ghost::moveReturning(Board& board) {
    // Retorna ao ponto de spawn
    int nextX = x, nextY = y;
    calculateNextMove(spawnX, spawnY, board, nextX, nextY);
    x = nextX;
    y = nextY;

    // Se chegou ao spawn, volta ao estado normal
    if (x == spawnX && y == spawnY) {
        state = GhostState::NORMAL;
        updateDisplay();
    }
}

void Ghost::makeVulnerable() {
    if (state != GhostState::RETURNING) {
        state = GhostState::VULNERABLE;
        vulnerableTimer = 300;
        updateDisplay();
    }
}

void Ghost::recover() {
    if (state == GhostState::VULNERABLE) {
        state = GhostState::NORMAL;
        vulnerableTimer = 0;
        updateDisplay();
    }
}

void Ghost::respawn() {
    x = spawnX;
    y = spawnY;
    state = GhostState::WAITING;
    isActive = true;
    updateDisplay();
}

void Ghost::draw() {
    attron(COLOR_PAIR(colorPair));
    mvaddch(y, x, ghostChar);
    attroff(COLOR_PAIR(colorPair));
}

void Ghost::updateDisplay() {
    switch (state) {
    case GhostState::VULNERABLE:
        ghostChar = 'v';
        colorPair = 6;  // Branco para vulnerável
        break;
    case GhostState::RETURNING:
        ghostChar = 'x';
        colorPair = 6;  // Branco para retornando
        break;
    default:
        switch (type) {
        case GhostType::BLINKY:
            ghostChar = 'B';
            colorPair = 2;  // Vermelho
            break;
        case GhostType::PINKY:
            ghostChar = 'P';
            colorPair = 3;  // Magenta
            break;
        case GhostType::INKY:
            ghostChar = 'I';
            colorPair = 4;  // Cyan
            break;
        case GhostType::CLYDE:
            ghostChar = 'C';
            colorPair = 5;  // Verde
            break;
        }
    }
}

// Implementação dos métodos de movimento específicos dos fantasmas
// (moveBlinky, movePinky, moveInky, moveClyde continuam os mesmos)

bool Ghost::canMoveTo(int newX, int newY, Board& board) {
    return board.isValidPosition(newX, newY);
}

void Ghost::calculateNextMove(int targetX, int targetY, Board& board, int& nextX, int& nextY) {
    int bestX = nextX;
    int bestY = nextY;
    int bestDistance = std::abs(targetX - nextX) + std::abs(targetY - nextY);

    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int newX = nextX + dx[i];
        int newY = nextY + dy[i];

        if (canMoveTo(newX, newY, board)) {
            int distance = std::abs(targetX - newX) + std::abs(targetY - newY);
            if (distance < bestDistance) {
                bestDistance = distance;
                bestX = newX;
                bestY = newY;
            }
        }
    }

    nextX = bestX;
    nextY = bestY;
}