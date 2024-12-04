#include <pdcurses/curses.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// Constantes de Tela
static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT = 24;

// Estrutura para representar uma posição no jogo
struct Position {
    int x, y;

    // Calcula distância entre duas posições
    double distanceTo(const Position& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// Classe representando o tabuleiro do jogo
class GameBoard {
private:
    char board[SCREEN_HEIGHT][SCREEN_WIDTH];
    int width, height;

public:
    GameBoard(int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT) : width(w), height(h) {
        // Inicializa o tabuleiro com um labirinto mais complexo
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Cria bordas e alguns obstáculos
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                    board[y][x] = '#';  // Parede externa
                }
                else if (
                    // Adiciona alguns obstáculos internos
                    (x % 10 == 0 && y % 5 == 0) ||  // Blocos regulares
                    (x + y) % 7 == 0  // Padrão diagonal
                    ) {
                    board[y][x] = '#';  // Parede interna
                }
                else {
                    board[y][x] = '.';  // Espaço vazio
                }
            }
        }
    }

    // Verifica se uma posição é válida
    bool isValidMove(int x, int y) const {
        return x > 0 && x < width - 1 && y > 0 && y < height - 1 &&
            board[y][x] != '#';
    }

    // Desenha o tabuleiro na tela
    void draw() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (board[y][x] == '#') {
                    // Destaca paredes
                    attron(COLOR_PAIR(2));  // Cor de parede
                    mvaddch(y, x, board[y][x]);
                    attroff(COLOR_PAIR(2));
                }
                else {
                    mvaddch(y, x, board[y][x]);
                }
            }
        }
    }

    // Adiciona informações de jogo no lado direito
    void drawGameInfo(int score, int lives) {
        // Área de informações (lado direito)
        int infoX = width + 2;
        mvprintw(2, infoX, "PAC-MAN GAME");
        mvprintw(4, infoX, "Score: %d", score);
        mvprintw(5, infoX, "Lives: %d", lives);
    }
};

// Classe Pac-Man (mantida similar)
class PacMan {
public:
    Position pos;
    int score;
    int lives;

    PacMan(int startX = 40, int startY = 12) : score(0), lives(3) {
        pos.x = startX;
        pos.y = startY;
    }

    void moveByInput(int input) {
        switch (input) {
        case KEY_UP:    pos.y = std::max(1, pos.y - 1); break;
        case KEY_DOWN:  pos.y = std::min(SCREEN_HEIGHT - 2, pos.y + 1); break;
        case KEY_LEFT:  pos.x = std::max(1, pos.x - 1); break;
        case KEY_RIGHT: pos.x = std::min(SCREEN_WIDTH - 2, pos.x + 1); break;
        }
    }

    void draw() {
        attron(COLOR_PAIR(3));  // Cor do Pac-Man
        mvaddch(pos.y, pos.x, 'C');
        attroff(COLOR_PAIR(3));
    }
};

// Classes de Estratégia (mantidas similares ao exemplo anterior)
class ChaseStrategy {
public:
    virtual Position calculateNextMove(
        const Position& ghostPos,
        const Position& pacmanPos,
        const GameBoard& board
    ) = 0;

    virtual ~ChaseStrategy() {}
};

class AggressiveChaseStrategy : public ChaseStrategy {
public:
    Position calculateNextMove(
        const Position& ghostPos,
        const Position& pacmanPos,
        const GameBoard& board
    ) override {
        Position nextPos = ghostPos;

        if (ghostPos.x < pacmanPos.x && board.isValidMove(ghostPos.x + 1, ghostPos.y)) {
            nextPos.x++;
        }
        else if (ghostPos.x > pacmanPos.x && board.isValidMove(ghostPos.x - 1, ghostPos.y)) {
            nextPos.x--;
        }

        if (ghostPos.y < pacmanPos.y && board.isValidMove(nextPos.x, ghostPos.y + 1)) {
            nextPos.y++;
        }
        else if (ghostPos.y > pacmanPos.y && board.isValidMove(nextPos.x, ghostPos.y - 1)) {
            nextPos.y--;
        }

        return nextPos;
    }
};

class RandomChaseStrategy : public ChaseStrategy {
public:
    Position calculateNextMove(
        const Position& ghostPos,
        const Position& pacmanPos,
        const GameBoard& board
    ) override {
        Position nextPos = ghostPos;

        int direction = rand() % 4;

        switch (direction) {
        case 0: // Cima
            if (board.isValidMove(ghostPos.x, ghostPos.y - 1))
                nextPos.y--;
            break;
        case 1: // Baixo
            if (board.isValidMove(ghostPos.x, ghostPos.y + 1))
                nextPos.y++;
            break;
        case 2: // Esquerda
            if (board.isValidMove(ghostPos.x - 1, ghostPos.y))
                nextPos.x--;
            break;
        case 3: // Direita
            if (board.isValidMove(ghostPos.x + 1, ghostPos.y))
                nextPos.x++;
            break;
        }

        return nextPos;
    }
};

// Classe Fantasma
class Ghost {
private:
    Position pos;
    ChaseStrategy* strategy;
    char displayChar;
    int color;

public:
    Ghost(int startX, int startY, ChaseStrategy* chaseStrategy, char symbol, int colorPair)
        : strategy(chaseStrategy), displayChar(symbol), color(colorPair) {
        pos.x = startX;
        pos.y = startY;
    }

    void chase(const PacMan& pacman, const GameBoard& board) {
        pos = strategy->calculateNextMove(pos, pacman.pos, board);
    }

    void draw() {
        attron(COLOR_PAIR(color));
        mvaddch(pos.y, pos.x, displayChar);
        attroff(COLOR_PAIR(color));
    }

    Position getPosition() const { return pos; }
};

// Função principal de demonstração
int main() {
    // Inicializa PDCurses
    initscr();            // Iniciar modo curses
    start_color();        // Habilita cores
    cbreak();             // Desabilita buffer de linha
    noecho();             // Não mostra teclas pressionadas
    keypad(stdscr, TRUE); // Permite teclas especiais
    curs_set(0);          // Esconde cursor
    srand(time(NULL));    // Seed para números aleatórios

    // Inicializa pares de cores
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // Cor padrão
    init_pair(2, COLOR_BLUE, COLOR_BLACK);    // Cor de paredes
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // Cor do Pac-Man
    init_pair(4, COLOR_RED, COLOR_BLACK);     // Cor do fantasma agressivo
    init_pair(5, COLOR_GREEN, COLOR_BLACK);   // Cor do fantasma aleatório

    // Cria objetos do jogo
    GameBoard board;
    PacMan pacman;

    // Cria estratégias de perseguição
    AggressiveChaseStrategy aggressiveStrategy;
    RandomChaseStrategy randomStrategy;

    // Cria fantasmas com estratégias e cores diferentes
    Ghost aggressiveGhost(20, 10, &aggressiveStrategy, 'A', 4);
    Ghost randomGhost(60, 10, &randomStrategy, 'R', 5);

    // Loop principal do jogo
    int ch;
    while ((ch = getch()) != 'q') {
        clear();  // Limpa a tela

        // Move Pac-Man
        pacman.moveByInput(ch);

        // Move fantasmas
        aggressiveGhost.chase(pacman, board);
        randomGhost.chase(pacman, board);

        // Desenha elementos
        board.draw();
        board.drawGameInfo(pacman.score, pacman.lives);
        pacman.draw();
        aggressiveGhost.draw();
        randomGhost.draw();

        refresh();  // Atualiza a tela
        napms(100);  // Pequeno delay
    }

    // Limpa PDCurses
    endwin();
    return 0;
}
