#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
public:
    // Aqui definimos os tipos possíveis de cada quadradinho do tabuleiro
   // É como uma lista de opções, onde cada opção tem um nome
    enum class SquareType {
        WALL,
        EMPTY,
        PELLET,
        POWER_PELLET
    };
    // Esta estrutura representa um quadradinho do tabuleiro
    // Cada quadradinho guarda algumas informações
    struct Square {
        SquareType type;  // Que tipo de quadrado é (parede, vazio, etc)
        int points;       // Quantos pontos vale (se for uma pastilha)
        bool powerActive; // Se a pastilha de poder está ativa
        int powerDuration;// Quanto tempo dura o poder

        Square();  // Construtor no cpp inicializa os valores quando cria um novo quadrado
    };

    // Construtor do tabuleiro - cria um novo tabuleiro com largura e altura especificadas
    Board(int w, int h);

    // Funções para modificar o tabuleiro
    void setSquare(int x, int y, SquareType type);  // Muda o tipo de um quadrado
    void removePellet(int x, int y);                // Remove uma pastilha do tabuleiro
    bool activatePowerPellet(int x, int y);         // Ativa uma pastilha de poder

    // Funções para obter informações do tabuleiro
    Square& getSquare(int x, int y);         // Pega um quadrado específico
    int getWidth();                          // Pega a largura do tabuleiro
    int getHeight();                         // Pega a altura do tabuleiro

private:
    int width;                                      // Largura do tabuleiro
    int height;                                     // Altura do tabuleiro
    std::vector<std::vector<Square>> squares;       // Matriz de quadrados que forma o tabuleiro
};

#endif 