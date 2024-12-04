#include "board.h"
#include <stdexcept>
#include <algorithm>

Board::Board(int w, int h)
    : width(31), height(28), totalPellets(0), remainingPellets(0), fruitActive(false) {
    squares.resize(height, std::vector<Square>(width));
    ghostSpawns.resize(4); // 4 fantasmas padrão
    initializeBoard();
}

Board::~Board() {
    clearBoard();
}

void Board::initializeBoard() {
    clearBoard();
    generateMaze();
    configureTunnels();
    updatePelletCount();

    if (!testTunnels()) {
        throw std::runtime_error("Tunnel system failed to initialize correctly");
    }
}

void Board::setSquare(int x, int y, SquareType type) {
    validatePosition(x, y);
    squares[y][x].type = type;
    if (type == SquareType::POWER_PELLET) {
        squares[y][x].points = 50;
        squares[y][x].powerDuration = 15;
    }
    else if (type == SquareType::PELLET) {
        squares[y][x].points = 10;
    }
}

void Board::removePellet(int x, int y) {
    validatePosition(x, y);
    if (squares[y][x].type == SquareType::PELLET ||
        squares[y][x].type == SquareType::POWER_PELLET) {
        squares[y][x].type = SquareType::EMPTY;
        remainingPellets--;
    }
}

bool Board::activatePowerPellet(int x, int y) {
    validatePosition(x, y);
    if (squares[y][x].type == SquareType::POWER_PELLET) {
        squares[y][x].powerActive = true;
        removePellet(x, y);
        return true;
    }
    return false;
}

Board::Square& Board::getSquare(int x, int y) {
    validatePosition(x, y);
    return squares[y][x];
}

bool Board::isValidPosition(int x, int y) const {
    return isPositionInBounds(x, y) && !isWall(x, y);
}

bool Board::isWall(int x, int y) const {
    validatePosition(x, y);
    return squares[y][x].type == SquareType::WALL;
}

bool Board::isPellet(int x, int y) const {
    validatePosition(x, y);
    return squares[y][x].type == SquareType::PELLET;
}

bool Board::isPowerPellet(int x, int y) const {
    validatePosition(x, y);
    return squares[y][x].type == SquareType::POWER_PELLET;
}

bool Board::isTunnel(int x, int y) const {
    validatePosition(x, y);
    return squares[y][x].isTunnel;
}

bool Board::isCompleted() const {
    return remainingPellets == 0;
}

void Board::setTunnel(int x1, int y1, int x2, int y2) {
    validatePosition(x1, y1);
    validatePosition(x2, y2);

    squares[y1][x1].isTunnel = true;
    squares[y1][x1].tunnelDestX = x2;
    squares[y1][x1].tunnelDestY = y2;

    squares[y2][x2].isTunnel = true;
    squares[y2][x2].tunnelDestX = x1;
    squares[y2][x2].tunnelDestY = y1;
}

void Board::getTunnelDestination(int x, int y, int& destX, int& destY) const {
    validatePosition(x, y);
    if (!squares[y][x].isTunnel) {
        throw std::runtime_error("Position is not a tunnel");
    }
    destX = squares[y][x].tunnelDestX;
    destY = squares[y][x].tunnelDestY;
}

void Board::setPacmanSpawn(int x, int y) {
    validatePosition(x, y);
    pacmanSpawn = SpawnPoint(x, y);
}

void Board::setGhostSpawn(int ghostIndex, int x, int y) {
    validatePosition(x, y);
    if (ghostIndex < 0 || ghostIndex >= static_cast<int>(ghostSpawns.size())) {
        throw std::out_of_range("Invalid ghost index");
    }
    ghostSpawns[ghostIndex] = SpawnPoint(x, y);
}

void Board::getSpawnPoint(int& x, int& y, bool isGhost, int ghostIndex) const {
    if (isGhost) {
        if (ghostIndex < 0 || ghostIndex >= static_cast<int>(ghostSpawns.size())) {
            throw std::out_of_range("Invalid ghost index");
        }
        x = ghostSpawns[ghostIndex].x;
        y = ghostSpawns[ghostIndex].y;
    }
    else {
        x = pacmanSpawn.x;
        y = pacmanSpawn.y;
    }
}

void Board::validatePosition(int x, int y) const {
    if (!isPositionInBounds(x, y)) {
        throw std::out_of_range("Position out of bounds");
    }
}

bool Board::isPositionInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Board::updatePelletCount() {
    totalPellets = 0;
    remainingPellets = 0;
    for (const auto& row : squares) {
        for (const auto& square : row) {
            if (square.type == SquareType::PELLET ||
                square.type == SquareType::POWER_PELLET) {
                totalPellets++;
                remainingPellets++;
            }
        }
    }
}

void Board::clearBoard() {
    for (auto& row : squares) {
        for (auto& square : row) {
            square = Square();
        }
    }
    totalPellets = 0;
    remainingPellets = 0;
    fruitActive = false;
}

void Board::generateMaze() {
    // Define o layout do mapa 28x31
    const std::string mazeLayout[] = {
        "#############################",
        "#            #            #",
        "#.####.#####.#.#####.####.#",
        "#o####.#####.#.#####.####o#",
        "#.####.#####.#.#####.####.#",
        "#...........................#",
        "#.####.#.#########.#.####.#",
        "#.####.#.#########.#.####.#",
        "#......#.....#.....#......#",
        "######.##### # #####.######",
        "     #.##### # #####.#     ",
        "     #.#           #.#     ",
        "     #.# ###-#### #.#     ",
        "######.# #       # #.######",
        "T     .  #       #  .     T", // Túneis nas extremidades
        "######.# #       # #.######",
        "     #.# ######### #.#     ",
        "     #.#           #.#     ",
        "     #.# ######### #.#     ",
        "######.# ######### #.######",
        "#            #            #",
        "#.####.#####.#.#####.####.#",
        "#.####.#####.#.#####.####.#",
        "#o..#........P........#..o#",
        "###.#.#.#########.#.#.#.###",
        "#......#.....#.....#......#",
        "#.##########.#.##########.#",
        "#.##########.#.##########.#",
        "#...........................#",
        "#############################"
    };

    // Preenche o tabuleiro baseado no layout
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (mazeLayout[y][x]) {
            case '#':
                squares[y][x].type = SquareType::WALL;
                break;
            case '.':
                squares[y][x].type = SquareType::PELLET;
                squares[y][x].points = 10;
                break;
            case 'o':
                squares[y][x].type = SquareType::POWER_PELLET;
                squares[y][x].points = 50;
                squares[y][x].powerDuration = 15;
                break;
            case 'P':
                setPacmanSpawn(x, y);
                squares[y][x].type = SquareType::PELLET;
                squares[y][x].points = 10;
                break;
            case '-':
                squares[y][x].type = SquareType::WALL;
                break;
            case 'T':  // Adicionando caso para túneis
                squares[y][x].type = SquareType::EMPTY;
                squares[y][x].isTunnel = true;
                break;
            case ' ':
                squares[y][x].type = SquareType::EMPTY;
                break;
            }
        }
    }

    // Define os spawn points dos fantasmas
    setGhostSpawn(0, 13, 11);  // Blinky
    setGhostSpawn(1, 13, 13);  // Pinky
    setGhostSpawn(2, 15, 13);  // Inky
    setGhostSpawn(3, 17, 13);  // Clyde
}

void Board::configureTunnels() {
    // Define os túneis nas posições y=14
    setTunnel(0, 14, width - 1, 14);
}

bool Board::testTunnels() const {
    // Verifica se os túneis estão configurados corretamente
    try {
        int destX, destY;
        getTunnelDestination(0, 14, destX, destY);
        if (destX != width - 1 || destY != 14) return false;

        getTunnelDestination(width - 1, 14, destX, destY);
        if (destX != 0 || destY != 14) return false;

        return true;
    }
    catch (...) {
        return false;
    }
}