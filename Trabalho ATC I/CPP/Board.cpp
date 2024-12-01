#include "board.h"
#include <curses.h>
#include <vector>

// Constantes para tipos de quadrados
const chtype WALL_CHAR = '#';
const chtype PELLET_CHAR = '.';
const chtype POWER_PELLET_CHAR = 'O';
const chtype EMPTY_CHAR = ' ';

Board::Board(int w, int h) : width(w), height(h) {
    // Inicializa a matriz de quadrados
    squares.resize(height, std::vector<Square>(width));
}

void Board::initializeBoard() {
    // Método para configurar o tabuleiro inicial
    // Exemplo de como pode ser criado
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Cria bordas
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                squares[y][x].type = SquareType::WALL;
            }
            // Distribui pastilhas
            else if (rand() % 10 < 3) {  // 30% de chance de ser pastilha
                squares[y][x].type = SquareType::PELLET;
                squares[y][x].points = 10;
            }
            else {
                squares[y][x].type = SquareType::EMPTY;
            }
        }
    }
}

void Board::drawBoard() {
    // Desenha o tabuleiro usando PDCurses
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (squares[y][x].type) {
            case SquareType::WALL:
                mvaddch(y, x, WALL_CHAR);
                break;
            case SquareType::PELLET:
                mvaddch(y, x, PELLET_CHAR);
                break;
            case SquareType::POWER_PELLET:
                mvaddch(y, x, POWER_PELLET_CHAR);
                break;
            case SquareType::EMPTY:
                mvaddch(y, x, EMPTY_CHAR);
                break;
            }
        }
    }
    refresh();  // Atualiza a tela no PDCurses
}

bool Board::isValidMove(int x, int y) {
    // Verifica se a posição é válida e não é uma parede
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return squares[y][x].type != SquareType::WALL;
}

void Board::collectPellet(int x, int y) {
    // Remove pastilha e retorna pontos
    if (squares[y][x].type == SquareType::PELLET) {
        int points = squares[y][x].points;
        squares[y][x].type = SquareType::EMPTY;
        // Aqui você pode passar os pontos para o Pacman
    }
    else if (squares[y][x].type == SquareType::POWER_PELLET) {
        squares[y][x].type = SquareType::EMPTY;
        // Lógica de pastilha de poder
    }
}

// Construtor do Square
Board::Square::Square()
    : type(SquareType::EMPTY),
    points(0),
    powerActive(false),
    powerDuration(0) {
}