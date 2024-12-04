#include "game.h"
#include <curses.h>

Game::Game(int width, int height)
    : board(new Board(width, height)),
    pacman(new Pacman(width / 2, height / 2)),
    gameMenu(new GameMenu("Pac-Man")),
    highscoreManager(new HighScoreManager()),
    state(GameState::MENU),
    currentLevel(1),
    score(0),
    lives(3),
    isGameOver(false),
    transitionTimer(0)
{
    // Inicializa fantasmas
    initializeGhosts();

    // Configura níveis
    initializeLevelConfigs();

    // Configura menu principal
    gameMenu->addOption("Novo Jogo", 10, 10, [this]() { startGame(); });
    gameMenu->addOption("Continuar", 10, 12, [this]() { resumeGame(); });
    gameMenu->addOption("Pontuações", 10, 14, [this]() { showHighScore(); });
    gameMenu->addOption("Sair", 10, 16, []() { exit(0); });
}

void Game::initializeGhosts() {
    // Cria os 4 fantasmas com diferentes comportamentos
    ghosts.push_back(std::make_shared<Ghost>(13, 11, GhostType::BLINKY));
    ghosts.push_back(std::make_shared<Ghost>(13, 13, GhostType::PINKY));
    ghosts.push_back(std::make_shared<Ghost>(15, 13, GhostType::INKY));
    ghosts.push_back(std::make_shared<Ghost>(17, 13, GhostType::CLYDE));
}

void Game::initializeLevelConfigs() {
    // Configura dificuldade progressiva para cada nível
    levelConfigs = {
        // Nível 1
        {1, 1, 300, 500},  // Velocidades normais, poder dura mais,Velocidade dos fantasmas normal,Velocidade do Pacman normal,Power pellet dura 10 segundos,Bônus de pontos ao completar
        // Nível 2
        {2, 1, 250, 1000}, // Fantasmas mais rápidos
        // Nível 3
        {2, 2, 200, 1500}, // Todos mais rápidos
        // Nível 4
        {3, 2, 150, 2000}, // Fantasmas ainda mais rápidos
        // Nível 5
        {3, 3, 100, 2500}  // Máxima velocidade
    };
}

void Game::startGame() {
    resetGameState();
    state = GameState::PLAYING;
    spawnEntities();
}

void Game::updateGameState() {
    if (state == GameState::PLAYING) {
        // Atualiza posição do Pacman
        pacman->move(*board);

        // Atualiza fantasmas
        updateGhosts();

        // Verifica colisões
        checkCollisions();

        // Verifica vitória
        checkVictoryCondition();
    }
    else if (state == GameState::TRANSITION) {
        if (--transitionTimer <= 0) {
            nextLevel();
        }
    }

    // Renderiza o estado atual
    renderGame();
}

void Game::handleInput(int input) {
    switch (state) {
    case GameState::PLAYING:
        switch (input) {
        case KEY_UP:
            pacman->changeDirection(0, -1);
            break;
        case KEY_DOWN:
            pacman->changeDirection(0, 1);
            break;
        case KEY_LEFT:
            pacman->changeDirection(-1, 0);
            break;
        case KEY_RIGHT:
            pacman->changeDirection(1, 0);
            break;
        case 'p':
        case 'P':
            pauseGame();
            break;
        }
        break;

    case GameState::PAUSED:
        if (input == 'p' || input == 'P') {
            resumeGame();
        }
        break;

    case GameState::MENU:
        gameMenu->handleInput(input);
        break;
    }
}

void Game::pauseGame() {
    if (state == GameState::PLAYING) {
        state = GameState::PAUSED;
        showPauseMenu();
    }
}

void Game::resumeGame() {
    if (state == GameState::PAUSED) {
        state = GameState::PLAYING;
    }
}

void Game::nextLevel() {
    currentLevel++;
    if (currentLevel <= static_cast<int>(levelConfigs.size())) {
        // Configura próximo nível
        resetLevel();
        updateDifficulty();
        state = GameState::PLAYING;
    }
    else {
        // Jogador venceu todos os níveis
        showGameOver();
    }
}

void Game::checkVictoryCondition() {
    if (board->isCompleted()) {
        // Nível completo
        addLevelBonus();
        state = GameState::LEVEL_COMPLETE;
        transitionTimer = 90; // 3 segundos a 30 fps
        showTransitionScreen();
    }
}

void Game::showTransitionScreen() {
    clear();
    mvprintw(10, 30, "Nível %d Completo!", currentLevel);
    mvprintw(12, 30, "Bônus: %d pontos", levelConfigs[currentLevel - 1].bonusPoints);
    mvprintw(14, 30, "Próximo nível em %d...", transitionTimer / 30);
    refresh();
}

void Game::updateDifficulty() {
    if (currentLevel <= static_cast<int>(levelConfigs.size())) {
        const auto& config = levelConfigs[currentLevel - 1];

        // Aplica configurações do nível
        for (auto& ghost : ghosts) {
            ghost->setSpeed(config.ghostSpeed);
        }
        pacman->setSpeed(config.pacmanSpeed);
    }
}

void Game::showPauseMenu() {
    clear();
    mvprintw(10, 35, "JOGO PAUSADO");
    mvprintw(12, 30, "Pressione P para continuar");
    mvprintw(14, 30, "Nível atual: %d", currentLevel);
    mvprintw(15, 30, "Pontuação: %d", score);
    refresh();
}

void Game::renderGame() {
    switch (state) {
    case GameState::PLAYING:
        // Desenha elementos do jogo
        board->draw();
        pacman->draw();
        for (const auto& ghost : ghosts) {
            ghost->draw();
        }

        // Informações na tela
        mvprintw(0, 0, "Nível: %d", currentLevel);
        mvprintw(0, 20, "Pontos: %d", score);
        mvprintw(0, 40, "Vidas: %d", lives);
        break;

    case GameState::PAUSED:
        showPauseMenu();
        break;

    case GameState::TRANSITION:
        showTransitionScreen();
        break;

    case GameState::GAME_OVER:
        showGameOver();
        break;
    }
    refresh();
}