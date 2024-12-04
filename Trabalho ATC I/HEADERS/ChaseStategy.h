#ifndef GHOST_TYPES_H
#define GHOST_TYPES_H

#include "board.h"
#include "ghost.h"
#include "pacman.h"
#include <vector>


class Ghost;
class Pacman;
class Board;


struct Position {
    int x, y;

    Position(int x, int y);
    double distanceTo(const Position& other) const;  
};


class ChaseStrategy {
protected:
    Board* gameBoard;

public:
    ChaseStrategy(Board* board);
    virtual ~ChaseStrategy() = default;

    
    virtual Position calculateNextPosition(
        const Ghost* ghost,
        const Pacman* pacman,
        const Board* board
    ) = 0;

protected:
    bool isValidPosition(int x, int y); 
};

// Different chase strategy implementations
class AggressiveChaseStrategy : public ChaseStrategy {
public:
    AggressiveChaseStrategy(Board* board);
    Position calculateNextPosition(
        const Ghost* ghost,
        const Pacman* pacman,
        const Board* board
    ) override;
};

class AmbushChaseStrategy : public ChaseStrategy {
public:
    AmbushChaseStrategy(Board* board);
    Position calculateNextPosition(
        const Ghost* ghost,
        const Pacman* pacman,
        const Board* board
    ) override;
};

class PatrolChaseStrategy : public ChaseStrategy {
public:
    PatrolChaseStrategy(Board* board);
    Position calculateNextPosition(
        const Ghost* ghost,
        const Pacman* pacman,
        const Board* board
    ) override;
};

class RandomChaseStrategy : public ChaseStrategy {
public:
    RandomChaseStrategy(Board* board);
    Position calculateNextPosition(
        const Ghost* ghost,
        const Pacman* pacman,
        const Board* board
    ) override;
};

// Base ghost class
class BaseGhost : public Ghost {
protected:
    ChaseStrategy* chaseStrategy;  // Chase strategy
    Board* gameBoard;              // Board reference

public:
    BaseGhost(int startX, int startY, Board* board, ChaseStrategy* strategy);
    virtual ~BaseGhost();
    void chase(Pacman* pacman) override;
};

// Specific ghost types
class Blinky : public BaseGhost {
public:
    Blinky(int startX, int startY, Board* board);
};

class Pinky : public BaseGhost {
public:
    Pinky(int startX, int startY, Board* board);
};

class Inky : public BaseGhost {
public:
    Inky(int startX, int startY, Board* board);
};

class Clyde : public BaseGhost {
public:
    Clyde(int startX, int startY, Board* board);
};

#endif // GHOST_TYPES_H