#include "pacman_ui.h"
#include <curses.h>
#include <string>
#include <iomanip>
#include <sstream>

void PacmanUI::initializeUI() {
    // inicia o PDCurses
    initscr();            // Start curses mode
    cbreak();             // Line buffering disabled
    noecho();             // Don't echo() while we do getch
    keypad(stdscr, TRUE); // We get F1, F2 etc..
    curs_set(0);          // Hide the cursor
    timeout(100);         // Non-blocking input with 100ms delay

    // Initialize colors
    if (has_colors() == FALSE) {
        endwin();
        throw std::runtime_error("Your terminal does not support color");
    }

    start_color();
    initializeColors();
}

void PacmanUI::initializeColors() {
    //inicia as cores
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // Pacman color
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Blinky color
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);  // Pinky color
    init_pair(4, COLOR_CYAN, COLOR_BLACK);     // Inky color
    init_pair(5, COLOR_GREEN, COLOR_BLACK);    // Clyde color
    init_pair(6, COLOR_WHITE, COLOR_BLACK);    // Default text color
    init_pair(7, COLOR_BLUE, COLOR_BLACK);     // Maze/Wall color
}

void PacmanUI::cleanupUI() {
    // End curses mode
    endwin();
}

void PacmanUI::drawPacman(const Pacman& pacman) {
    attron(COLOR_PAIR(1));
    mvaddch(pacman.getY(), pacman.getX(), 'C');
    attroff(COLOR_PAIR(1));
}

void PacmanUI::drawGhost(const Ghost& ghost) {
    int colorPair;
    char ghostChar = 'G';

    // Assign different colors based on ghost type (this would need to be enhanced 
    // with a way to distinguish ghost types)
    switch (ghost.getCurrentState()) {
    case GhostState::CHASE:
        colorPair = 2;  // Red
        break;
    case GhostState::FRIGHTENED:
        colorPair = 6;  // White (when scared)
        ghostChar = '!';
        break;
    default:
        colorPair = 3;  // Magenta
    }

    attron(COLOR_PAIR(colorPair));
    mvaddch(ghost.getY(), ghost.getX(), ghostChar);
    attroff(COLOR_PAIR(colorPair));
}

void PacmanUI::drawBoard(const Board& board) {
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            Board::Square& square = board.getSquare(x, y);

            switch (square.type) {
            case Board::SquareType::WALL:
                attron(COLOR_PAIR(7));
                mvaddch(y, x, '#');
                attroff(COLOR_PAIR(7));
                break;
            case Board::SquareType::PELLET:
                mvaddch(y, x, '.');
                break;
            case Board::SquareType::POWER_PELLET:
                attron(A_BOLD);
                mvaddch(y, x, 'O');
                attroff(A_BOLD);
                break;
            default:
                mvaddch(y, x, ' ');
            }
        }
    }
}

void PacmanUI::drawScore(const Pacman& pacman) {
    attron(COLOR_PAIR(6));
    mvprintw(0, 0, "Score: %d", pacman.getScore());
    attroff(COLOR_PAIR(6));
}

void PacmanUI::drawLives(const Pacman& pacman) {
    attron(COLOR_PAIR(1));
    mvprintw(0, SCREEN_WIDTH - 10, "Lives: %d", pacman.getLives());
    attroff(COLOR_PAIR(1));
}

void PacmanUI::drawGameOver(int finalScore) {
    clear();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(SCREEN_HEIGHT / 2 - 1, SCREEN_WIDTH / 2 - 5, "GAME OVER");
    attroff(COLOR_PAIR(2) | A_BOLD);

    attron(COLOR_PAIR(6));
    mvprintw(SCREEN_HEIGHT / 2 + 1, SCREEN_WIDTH / 2 - 10, "Final Score: %d", finalScore);
    attroff(COLOR_PAIR(6));

    refreshScreen();
    getch(); // Wait for key press
}

void PacmanUI::renderMenu(const GameMenu& menu) {
    clear();

    // Draw title
    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(3, SCREEN_WIDTH / 2 - menu.getTitle().length() / 2, "%s", menu.getTitle().c_str());
    attroff(COLOR_PAIR(6) | A_BOLD);

    // Draw menu options
    const auto& options = menu.getOptions();
    for (size_t i = 0; i < options.size(); ++i) {
        if (menu.getSelectedIndex() == static_cast<int>(i)) {
            attron(A_REVERSE);
        }
        mvprintw(10 + i * 2, SCREEN_WIDTH / 2 - options[i]->getLabel().length() / 2,
            "%s", options[i]->getLabel().c_str());
        if (menu.getSelectedIndex() == static_cast<int>(i)) {
            attroff(A_REVERSE);
        }
    }

    refreshScreen();
}

void PacmanUI::drawHighScores(const std::list<ScoreEntry>& scores) {
    clear();

    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(3, SCREEN_WIDTH / 2 - 5, "HIGH SCORES");
    attroff(COLOR_PAIR(6) | A_BOLD);

    int y = 6;
    for (const auto& entry : scores) {
        mvprintw(y, SCREEN_WIDTH / 2 - 10,
            "%-10s %5d", entry.playerName.c_str(), entry.score);
        y++;
    }

    refreshScreen();
    getch(); // Wait for key press
}

void PacmanUI::displayMessage(int y, int x, const std::string& message) {
    attron(COLOR_PAIR(6));
    mvprintw(y, x, "%s", message.c_str());
    attroff(COLOR_PAIR(6));
}

void PacmanUI::clearScreen() {
    clear();
}

void PacmanUI::refreshScreen() {
    refresh();
}

int PacmanUI::getInput() {
    return getch();
}