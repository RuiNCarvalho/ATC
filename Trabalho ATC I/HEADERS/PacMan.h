#ifndef PACMAN_H
#define PACMAN_H

#include "game_object.h"

class Pacman : public GameObject {
private:
    int direction_x;  // Movimento horizontal
    int direction_y;  // movimento vertical
    int lives;        // Numero de vidas
    int score;        // Player score

public:
    // Construtor
    Pacman(int startX, int startY);

    
    void move() ;
    
    void changeDirection(int dx, int dy);//Permite alterar a dire��o do movimento do Pac-Man.
    void collectPellet();                //incrementa a pontua��o do Pac-Man.
    void loseLife();                     //Diminui o n�mero de vidas do Pac-Man Quando apanhado pelo ghost

  
    int getLives() const;   //obtem numero atualizado de vidas do pacman  << "Vidas restantes: " << pacman.getLives() << "\n";
    int getScore() const;   //obtem o score atualizada de pontos do jogador  << "Pontua��o: " << pacman.getScore() << "\n";
};

#endif // PACMAN_H