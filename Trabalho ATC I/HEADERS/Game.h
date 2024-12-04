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

// Estados possíveis do jogo
enum class GameState {
    MENU,           // Menu principal
    PLAYING,        // Jogando
    PAUSED,         // Jogo pausado
    LEVEL_COMPLETE, // Nível completado
    GAME_OVER,      // Fim de jogo
    TRANSITION      // Transição entre níveis
};

class Game {
private:
    // Componentes principais do jogo
    Board* board;                                // Tabuleiro
    Pacman* pacman;                             // O Pacman
    std::vector<std::shared_ptr<Ghost>> ghosts; // Lista de fantasmas
    GameMenu* gameMenu;                         // Menu do jogo
    HighScoreManager* highscoreManager;         // Gerenciador de pontuação

    // Estado e controle do jogo
    GameState state;         // Estado atual do jogo
    int currentLevel;        // Nível atual
    int score;              // Pontuação
    int addLevelBonus       // aumenta ao passar de nivel
    int lives;              // Vidas restantes
    bool isGameOver;        // Se o jogo acabou
    int transitionTimer;    // Timer para telas de transição

    // Configurações do nível
    struct LevelConfig {
        int ghostSpeed;           // Velocidade dos fantasmas
        int pacmanSpeed;          // Velocidade do Pacman
        int powerPelletDuration; // Duração do power pellet
        int bonusPoints;         // Pontos bônus do nível
    };
    std::vector<LevelConfig> levelConfigs; // Configurações de cada nível

    class Game {
    private:
        // Componentes principais do jogo
        Board* board;
        Pacman* pacman;
        std::vector<std::shared_ptr<Ghost>> ghosts;
        GameMenu* gameMenu;
        HighScoreManager* highscoreManager;

        // Estado e controle do jogo
        GameState state;
        int currentLevel;
        int score;
        int addLevelBonus;
        int lives;
        bool isGameOver;
        int transitionTimer;

        // Configurações do nível
        struct LevelConfig {
            int ghostSpeed;
            int pacmanSpeed;
            int powerPelletDuration;
            int bonusPoints;
        };
        std::vector<LevelConfig> levelConfigs;

        // Sistema de Colisões
        struct CollisionResult {
            bool hitGhost;
            bool ghostVulnerable;
            bool hitPellet;
            bool hitPowerPellet;
        };
        CollisionResult checkCollisionAt(int x, int y);
    };

public:
    // Construtor e destrutor
    Game(int width, int height);
    ~Game();

    // Controle principal do jogo
    void startGame();              // Inicia novo jogo
    void updateGameState();        // Atualiza estado do jogo
    void handleInput(int input);   // Processa entrada do usuário

    // Controle de estados
    void pauseGame();             // Pausa o jogo
    void resumeGame();            // Retoma o jogo
    void nextLevel();             // Avança para próximo nível
    void resetLevel();            // Reinicia nível atual

    // Verificações de estado
    void checkCollisions();       // Verifica colisões
    void checkVictoryCondition(); // Verifica se ganhou
    bool isLevelComplete();       // Verifica se completou nível

    // Sistema de pontuação
    void incrementScore(int points);
    void addLevelBonus();

    // Sistema de vidas
    void loseLife();
    void addLife();

    // Menus e telas
    void showMainMenu();
    void showPauseMenu();
    void showTransitionScreen();
    void showGameOver();

private:
    // Métodos auxiliares
    void initializeLevelConfigs();    // Configura níveis
    void updateDifficulty();          // Atualiza dificuldade
    void resetGameState();            // Reseta estado do jogo
    void spawnEntities();             // Posiciona entidades
    void updateGhosts();              // Atualiza fantasmas
    void renderGame();                // Renderiza o jogo
};

#endif