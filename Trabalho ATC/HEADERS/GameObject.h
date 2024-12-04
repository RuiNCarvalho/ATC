#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
protected:
   
    int x, y;       // Posição do objeto no plano 
    int width, height;  // Dimensões do objeto
    int speed;      // Velocidade de movimento

public:
    // construtor
    GameObject(int startX, int startY, int objWidth, int objHeight, int moveSpeed);

   
    virtual void move() = 0; //movimento do objeto 

    //Acessar os valores das propriedades 
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
};

#endif