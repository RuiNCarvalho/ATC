#ifndef GHOST_H    
#define GHOST_H

#include "game_object.h"   
#include "pacman.h"         


enum class GhostState {
    CHASE,      // Estado de perseguição ao Pacman
    SCATTER,    // Estado de dispersão 
    FRIGHTENED, // Estado assustado 
    RETURNING   // Estado de retorno à casa dos fantasmas
};


class GhostPersonality;


class Ghost : public GameObject {
private:
    
    GhostState currentState;           // Estado atual do fantasma
    GhostPersonality* personality;     //apontador para a personalidade do fantasma po~is cada fantasma tem um comportamento único
    

public:
    // Construtor da classe que Recebe as coordenadas iniciais do fantasma
   
    Ghost(int startX, int startY);

   
    
    virtual ~Ghost();

  
    //comportamento do fantasma
    void chase(Pacman* pacman);    // Método para perseguir o Pacman
    void scatter();                // Método para entrar em modo de dispersão
    void frightenedMode();         // Método para entrar em modo assustado

    void move();                       
    void resetPosition();    // Método para resetar posição do fantasma
  
    // Getters e Setters 
    GhostState getCurrentState() const;           // Retorna o estado atual
    void setCurrentState(G hostState newState);    // Define um novo estado

    GhostPersonality* getPersonality() const;                 // Retorna a personalidade
    void setPersonality(GhostPersonality* newPersonality);    // Define nova personalidade
};

#endif //GHOST