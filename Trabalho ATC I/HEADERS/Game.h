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
    int score;                                   // Pontua��o atual
    int lives;                                   // Vidas restantes
    GameMenu* gameMenu;                          // Menu do jogo (para mostrar menus)
    HighScoreManager* highscoreManager;          // Gerenciador de pontua��es altas
    bool isGameOver;                             // Flag para o fim do jogo

public:
    // Construtor e destrutor
    Game(int width, int height);
    ~Game();

    // Inicia o jogo
    void startGame();

    // Finaliza o jogo
    void endGame();

    // Atualiza o estado do jogo (movimenta personagens, verifica colis�es, etc.)
    void updateGameState();

    // Verifica colis�es entre o Pacman e os fantasmas ou outros objetos
    void checkCollision();

    // Incrementa a pontua��o
    void incrementScore(int points);

    // Diminui as vidas do Pacman
    void loseLife();

    //menu principal
    void showMainMenu();

    // pontua��o alta
    void showHighScore();

    // Fun��es auxiliares
    void reset();        // Reseta o jogo
    void gameOver();     // Define que o jogo acabou

    // Getters e Setters
    int getScore() const;
    int getLives() const;
    bool getIsGameOver() const;
};

#endif // GAME_H
