#include "pacman.h"
#include <curses.h>

// Construtor
Pacman::Pacman(int startX, int startY)
    : GameObject(startX, startY, 1, 1, 1),  // Tamanho 1x1, velocidade 1
    direction_x(0), direction_y(0),        // Começa parado
    spawn_x(startX), spawn_y(startY),      // Guarda posição inicial
    lives(3),                              // Começa com 3 vidas
    score(0),                              // Começa com 0 pontos
    isPowered(false),                      // Começa sem power pellet
    powerTimer(0),                         // Timer do poder zerado
    pacmanChar('C')                        // Caractere 'C' representa o Pacman
{
}

// Movimento do Pacman
void Pacman::move(Board& board) {
    // Calcula nova posição baseada na direção
    int newX = x + (direction_x * speed);
    int newY = y + (direction_y * speed);

    // Só move se a nova posição for válida
    if (canMoveTo(newX, newY, board)) {
        x = newX;
        y = newY;
    }

    // Atualiza estado do power pellet
    updatePowerState();
}

// Verifica se pode mover para uma posição
bool Pacman::canMoveTo(int newX, int newY, Board& board) {
    // Checa se a posição está dentro do tabuleiro e não é parede
    return board.isValidPosition(newX, newY) && !board.isWall(newX, newY);
}

// Muda a direção do movimento
void Pacman::changeDirection(int dx, int dy) {
    direction_x = dx;
    direction_y = dy;
}

// Coleta pellet normal
void Pacman::collectPellet(int points) {
    score += points;
}

// Coleta power pellet
void Pacman::collectPowerPellet() {
    isPowered = true;
    powerTimer = 300;  // ~10 segundos de poder
    score += 50;       // Power pellets valem mais pontos
}

// Perde uma vida
void Pacman::loseLife() {
    if (lives > 0) {
        lives--;
        respawn();  // Volta para posição inicial
    }
}

// Volta para posição inicial
void Pacman::respawn() {
    // Reseta posição
    x = spawn_x;
    y = spawn_y;

    // Reseta direção
    direction_x = 0;
    direction_y = 0;

    // Reseta poder
    isPowered = false;
    powerTimer = 0;
}

// Verifica se power pellet está ativo
bool Pacman::isPowerPelletActive() const {
    return isPowered && powerTimer > 0;
}

// Atualiza estado do power pellet
void Pacman::updatePowerState() {
    if (isPowered) {
        if (--powerTimer <= 0) {
            isPowered = false;  // Poder acabou
        }
    }
}

//Verifica as colissoes do pacman com as paredes
void Pacman::move(Board& board) {
    // Calcula nova posição baseada na direção atual
    int newX = x + (direction_x * speed);
    int newY = y + (direction_y * speed);

    // Só move se não houver parede
    if (canwalk(novoX, newY, board)) {
        x = newX;
        y = newY;

        // Verifica se pegou pastilha
        if (board.isPellet(x, y)) {
            collectPellet(10);
            board.removePellet(x, y);
        }
        // Verifica se pegou power pellet
        else if (board.isPowerPellet(x, y)) {
            collectPowerPellet();
            board.removePellet(x, y);
        }
    }
}

bool Pacman::canwalk(int newX, int newY, const Board& board) {
    // Verifica limites do tabuleiro
    if (newX < 0 || newX >= board.getWidth() ||
        newY < 0 || newY >= board.getHeight()) {
        return false;
    }

    // Verifica se tem parede
    return !board.isWall(newX, newY);
}



// Desenha o Pacman
void Pacman::draw() {
    attron(COLOR_PAIR(1));  // Usa cor amarela (definida na PacmanUI)
    mvaddch(y, x, pacmanChar);
    attroff(COLOR_PAIR(1));
}

// Getters
int Pacman::getLives() const {
    return lives;
}

int Pacman::getScore() const {
    return score;
}

int Pacman::getDirectionX() const {
    return direction_x;
}

int Pacman::getDirectionY() const {
    return direction_y;
}