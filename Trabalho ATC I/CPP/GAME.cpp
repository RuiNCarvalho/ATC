// game.cpp
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
    initializeGhosts();
    initializeLevelConfigs();
    setupMainMenu();
}

Game::~Game() {
    delete board;
    delete pacman;
    delete gameMenu;
    delete highscoreManager;
    ghosts.clear();
}

void Game::initializeGhosts() {
    ghosts.push_back(std::make_shared<Ghost>(13, 11, GhostType::BLINKY));
    ghosts.push_back(std::make_shared<Ghost>(13, 13, GhostType::PINKY));
    ghosts.push_back(std::make_shared<Ghost>(15, 13, GhostType::INKY));
    ghosts.push_back(std::make_shared<Ghost>(17, 13, GhostType::CLYDE));
}

void Game::initializeLevelConfigs() {
    levelConfigs = {
        {1, 1, 300, 500},  // Nível 1
        {2, 1, 250, 1000}, // Nível 2
        {2, 2, 200, 1500}  // Nível 3
    };
}

void Game::setupMainMenu() {
    gameMenu->addOption("Novo Jogo", 10, 10, [this]() { startGame(); });
    gameMenu->addOption("Continuar", 10, 12, [this]() { resumeGame(); });
    gameMenu->addOption("Pontuações", 10, 14, [this]() { showHighScore(); });
    gameMenu->addOption("Sair", 10, 16, []() { exit(0); });
}

void Game::startGame() {
    resetGameState();
    state = GameState::PLAYING;
    spawnEntities();
}

void Game::resetGameState() {
    score = 0;
    lives = 3;
    currentLevel = 1;
    isGameOver = false;
    board->resetBoard();
    pacman->respawn();
    for (auto& ghost : ghosts) {
        ghost->respawn();
    }
}

void Game::updateGameState() {
    if (state == GameState::PLAYING) {
        pacman->move(*board);
        updateGhosts();
        checkCollisions();
        checkVictoryCondition();
    }
    else if (state == GameState::TRANSITION) {
        if (--transitionTimer <= 0) {
            nextLevel();
        }
    }
    renderGame();
}

void Game::updateGhosts() {
    for (auto& ghost : ghosts) {
        ghost->move(pacman->getX(), pacman->getY(), *board);
    }
}

void Game::checkCollisions() {
    CollisionResult result = checkCollisionAt(pacman->getX(), pacman->getY());

    if (result.hitGhost) {
        if (pacman->isPowerPelletActive()) {
            incrementScore(200);
            for (auto& ghost : ghosts) {
                if (ghost->getX() == pacman->getX() && ghost->getY() == pacman->getY()) {
                    ghost->respawn();
                    break;
                }
            }
        }
        else {
            loseLife();
            if (lives <= 0) {
                state = GameState::GAME_OVER;
            }
            else {
                resetLevel();
            }
        }
    }

    if (result.hitPellet) {
        board->removePellet(pacman->getX(), pacman->getY());
        incrementScore(10);
    }

    if (result.hitPowerPellet) {
        board->removePellet(pacman->getX(), pacman->getY());
        incrementScore(50);
        for (auto& ghost : ghosts) {
            ghost->makeVulnerable();
        }
    }
}

void Game::checkVictoryCondition() {
    if (isLevelComplete()) {
        state = GameState::LEVEL_COMPLETE;
        addLevelBonus();
        transitionTimer = 90;
        showTransitionScreen();
    }
}

bool Game::isLevelComplete() {
    return board->getRemainingPellets() == 0;
}

Game::CollisionResult Game::checkCollisionAt(int x, int y) {
    CollisionResult result = { false, false, false, false };

    for (const auto& ghost : ghosts) {
        if (ghost->getX() == x && ghost->getY() == y) {
            result.hitGhost = true;
            result.ghostVulnerable = ghost->isVulnerable();
            break;
        }
    }

    if (board->isPellet(x, y)) {
        result.hitPellet = true;
    }
    else if (board->isPowerPellet(x, y)) {
        result.hitPowerPellet = true;
    }

    return result;
}

void Game::handleInput(int input) {
    switch (state) {
    case GameState::PLAYING:
        handlePlayingInput(input);
        break;
    case GameState::PAUSED:
        handlePausedInput(input);
        break;
    case GameState::MENU:
        gameMenu->handleInput(input);
        break;
    }
}

void Game::handlePlayingInput(int input) {
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
}

void Game::handlePausedInput(int input) {
    if (input == 'p' || input == 'P') {
        resumeGame();
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
    if (currentLevel <= 3) {
        resetLevel();
        updateDifficulty();
        state = GameState::PLAYING;
    }
    else {
        showGameOver();
    }
}

void Game::resetLevel() {
    board->resetBoard();
    pacman->respawn();
    for (auto& ghost : ghosts) {
        ghost->respawn();
    }
}

void Game::updateDifficulty() {
    const auto& config = levelConfigs[currentLevel - 1];
    for (auto& ghost : ghosts) {
        ghost->setSpeed(config.ghostSpeed);
    }
    pacman->setSpeed(config.pacmanSpeed);
}

void Game::incrementScore(int points) {
    score += points;
}

void Game::addLevelBonus() {
    score += levelConfigs[currentLevel - 1].bonusPoints;
}

void Game::loseLife() {
    lives--;
}




void Game::renderGame() {
    switch (state) {
    case GameState::PLAYING:
        board->draw();
        pacman->draw();
        for (const auto& ghost : ghosts) {
            ghost->draw();
        }
        drawHUD();
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

void Game::drawHUD() {
    mvprintw(0, 0, "Nível: %d", currentLevel);
    mvprintw(0, 20, "Pontos: %d", score);
    mvprintw(0, 40, "Vidas: %d", lives);
}

void Game::showGameOver() {
    clear();
    mvprintw(10, 30, "GAME OVER");
    mvprintw(12, 30, "Pontuação Final: %d", score);
    highscoreManager->addScore("Player", score);
    refresh();
}

void Game::showTransitionScreen() {
    clear();
    mvprintw(10, 30, "Nível %d Completo!", currentLevel);
    mvprintw(12, 30, "Bônus: %d pontos", levelConfigs[currentLevel - 1].bonusPoints);
    mvprintw(14, 30, "Próximo nível em %d...", transitionTimer / 30);
    refresh();
}

void Game::showPauseMenu() {
    clear();
    mvprintw(10, 35, "JOGO PAUSADO");
    mvprintw(12, 30, "Pressione P para continuar");
    mvprintw(14, 30, "Nível atual: %d", currentLevel);
    mvprintw(15, 30, "Pontuação: %d", score);
    refresh();
}

void Game::showHighScore() {
    auto scores = highscoreManager->getTopScores();
    clear();
    int y = 5;
    mvprintw(3, 30, "MAIORES PONTUAÇÕES");
    for (const auto& entry : scores) {
        mvprintw(y++, 25, "%s: %d", entry.playerName.c_str(), entry.score);
    }
    refresh();
    getch();
}