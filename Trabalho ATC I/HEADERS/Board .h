#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include <chrono>
#include <curses.h>

class Board {
public:
    enum class SquareType {
        WALL,
        EMPTY,
        PELLET,
        POWER_PELLET,
        TUNNEL,
        FRUIT_SPAWN
    };

    struct Square {
        SquareType type;
        int points;
        bool powerActive;
        int powerDuration;
        bool isTunnel;
        int tunnelDestX;
        int tunnelDestY;

        Square() : type(SquareType::EMPTY), points(0), powerActive(false),
            powerDuration(0), isTunnel(false), tunnelDestX(-1), tunnelDestY(-1) {}
    };

    // Construtor e Destrutor
    Board(int w = 31, int h = 28); // Dimensões padrão do Pacman
    ~Board();

    // Métodos de manipulação do tabuleiro
    void setSquare(int x, int y, SquareType type);
    void removePellet(int x, int y);
    bool activatePowerPellet(int x, int y);
    Square& getSquare(int x, int y);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Métodos de estado do jogo
    void initializeBoard();
    void resetBoard();
    bool isValidPosition(int x, int y) const;
    bool isWall(int x, int y) const;
    bool isPellet(int x, int y) const;
    bool isPowerPellet(int x, int y) const;
    bool isTunnel(int x, int y) const;
    bool isCompleted() const;

    // Métodos de túnel
    void setTunnel(int x1, int y1, int x2, int y2);
    void getTunnelDestination(int x, int y, int& destX, int& destY) const;
    bool testTunnels() const;
   
    // Métodos de spawn
    void setPacmanSpawn(int x, int y);
    void setGhostSpawn(int ghostIndex, int x, int y);
    void getSpawnPoint(int& x, int& y, bool isGhost = false, int ghostIndex = 0) const;

    // Métodos de contagem
    int getRemainingPellets() const { return remainingPellets; }
    int getTotalPellets() const { return totalPellets; }
    double getCompletionPercentage() const {
        return totalPellets > 0 ? (100.0 * (totalPellets - remainingPellets)) / totalPellets : 0;
    }

private:
    int width;
    int height;
    std::vector<std::vector<Square>> squares;
    int totalPellets;
    int remainingPellets;
    bool fruitActive;

    struct SpawnPoint {
        int x, y;
        SpawnPoint() : x(0), y(0) {}
        SpawnPoint(int _x, int _y) : x(_x), y(_y) {} 
    };

    SpawnPoint pacmanSpawn;
    std::vector<SpawnPoint> ghostSpawns;

    // Métodos privados
    void validatePosition(int x, int y) const;
    void updatePelletCount();
    bool isPositionInBounds(int x, int y) const;
    void clearBoard();
    void generateMaze();
    void configureTunnels();
};

#endif