#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "pacman.h"
#include "ghost.h"
#include "pacman_ui.h"
#include "game_menu.h"           
#include "highscore_manager.h"   
#include <vector>
#include <memory>

class Game {
private:
    Board* board;                                // Tabuleiro do jogo
    Pacman* pacman;                              // O Pacman
    std::vector<std::shared_ptr<Ghost>> ghosts;  // Lista de fantasmas
    int score;                                   // Pontuação atual
    int lives;                                   // Vidas restantes
    GameMenu* gameMenu;                          // Menu do jogo (para mostrar menus)
    HighScoreManager* highscoreManager;          // Gerenciador de pontuações altas
    bool isGameOver;                             // Flag para o fim do jogo

public:
    // Construtor e destrutor
    Game(int width, int height);
    ~Game();

    // Inicia o jogo
    void startGame();

    // Finaliza o jogo
    void endGame();

    // Atualiza o estado do jogo (movimenta personagens, verifica colisões, etc.)
    void updateGameState();

    // Verifica colisões entre o Pacman e os fantasmas ou outros objetos
    void checkCollision();

    // Incrementa a pontuação
    void incrementScore(int points);

    // Diminui as vidas do Pacman
    void loseLife();

    //menu principal
    void showMainMenu();

    // pontuação alta
    void showHighScore();

    // Funções auxiliares
    void reset();        // Reseta o jogo
    void gameOver();     // Define que o jogo acabou

    // Getters e Setters
    int getScore() const;
    int getLives() const;
    bool getIsGameOver() const;
};

#endif // GAME_H
