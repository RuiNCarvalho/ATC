#ifndef PACMAN_UI_H
#define PACMAN_UI_H

#include <curses.h>
#include <string>
#include <vector>
#include "pacman.h"
#include "ghost.h"
#include "board.h"
#include "game_menu.h"
#include "highscore_manager.h"

class PacmanUI {
public:
    // Dimensões do tabuleiro em tiles
    static const int BOARD_WIDTH = 31;
    static const int BOARD_HEIGHT = 28;

    // Dimensões da janela
    static const int WINDOW_WIDTH = 50;  // Espaço extra para pontuação e informações
    static const int WINDOW_HEIGHT = 32;  // Espaço extra para status

    // Caracteres para desenho
    static const chtype WALL_CHAR = '#';
    static const chtype PELLET_CHAR = '.';
    static const chtype POWER_PELLET_CHAR = 'O';
    static const chtype EMPTY_CHAR = ' ';

    // Inicialização e limpeza
    static void initializeUI();
    static void cleanupUI();

    // Desenho dos elementos do jogo
    static void drawGame(const Board& board, const Pacman& pacman,
        const std::vector<std::shared_ptr<Ghost>>& ghosts,
        int level, bool isPaused);

    // Animações e efeitos visuais
    static void playDeathAnimation(int x, int y);
    static void showPowerPelletEffect(bool active);
    static void showPointsCollected(int x, int y, int points);
    static void showLevelComplete(int level, int score);

    // Menus e telas especiais
    static void showTutorial();
    static void showGameOver(int finalScore);
    static void showPauseScreen();
    static void showVictoryScreen(int finalScore);

    // Métodos de utilidade
    static void refreshUI();
    static void clearScreen();
    static int getInput();

private:
    // Métodos de desenho interno
    static void drawBoard(const Board& board);
    static void drawPacman(const Pacman& pacman);
    static void drawGhost(const Ghost& ghost);
    static void drawScore(int score);
    static void drawLives(int lives);
    static void drawLevel(int level);
    static void drawPowerPelletTimer(int timeLeft);
    static void drawStatusBar(const Pacman& pacman, int level);

    // Animações
    static const std::vector<std::string> DEATH_ANIMATION;
    static const std::vector<std::string> VICTORY_ANIMATION;
    static void playAnimation(int x, int y, const std::vector<std::string>& frames);

    // Cores
    static void initializeColors();
    static const int PACMAN_COLOR = 1;
    static const int GHOST_NORMAL_COLOR = 2;
    static const int GHOST_VULNERABLE_COLOR = 3;
    static const int WALL_COLOR = 4;
    static const int PELLET_COLOR = 5;
    static const int TEXT_COLOR = 6;
    static const int HIGHLIGHT_COLOR = 7;
};

#endif