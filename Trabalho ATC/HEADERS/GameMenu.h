#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <string>
#include <vector>
#include <functional>
#include <curses.h>
#include "game_object.h"

// Define as dire��es poss�veis no menu
enum class MenuNavigation {
    UP,     // Seta para cima
    DOWN,   // Seta para baixo
    ENTER,  // Tecla Enter
    ESCAPE  // Tecla Esc
};

// Estrutura para representar cada op��o do menu
struct MenuItem {
    std::string label;           // Texto da op��o
    std::string description;     // Descri��o/explica��o da op��o
    std::function<void()> action;// Fun��o a ser executada
    bool isEnabled;              // Se a op��o est� habilitada

    MenuItem(const std::string& lbl, const std::string& desc,
        std::function<void()> act, bool enabled = true)
        : label(lbl), description(desc), action(act), isEnabled(enabled) {}
};

class GameMenu {
private:
    std::string title;                    // T�tulo do menu
    std::vector<MenuItem> menuItems;      // Lista de op��es do menu
    int selectedOption;                   // �ndice da op��o selecionada
    bool isActive;                        // Se o menu est� ativo
    bool showHelp;                        // Se mostra descri��es das op��es

    // Dimens�es e posi��o do menu
    int startX;
    int startY;
    int width;
    int height;

public:
    // Construtor: inicializa o menu
    GameMenu(const std::string& menuTitle);

    // Adiciona uma nova op��o ao menu
    void addMenuItem(const std::string& label,
        const std::string& description,
        std::function<void()> action,
        bool enabled = true);

    // Fun��es principais do menu
    void display();                           // Mostra o menu na tela
    void handleInput(int key);                // Processa entrada do usu�rio
    void selectCurrentOption();               // Executa op��o selecionada

    // Menus espec�ficos
    void showMainMenu();                      // Menu principal
    void showPauseMenu();                     // Menu de pausa
    void showConfigMenu();                    // Menu de configura��es
    void showControlsMenu();                  // Menu de controles
    void showConfirmExit();                   // Confirma��o de sa�da

private:
    // Fun��es auxiliares de desenho
    void drawFrame();                         // Desenha borda do menu
    void drawTitle();                         // Desenha t�tulo
    void drawOptions();                       // Desenha op��es
    void drawDescription();                   // Desenha descri��o da op��o atual
    void drawControls();                      // Desenha teclas de controle
    void clearMenuArea();                     // Limpa �rea do menu

    // Fun��es de navega��o
    void moveSelection(MenuNavigation dir);   // Move sele��o
    void updateSelection();                   // Atualiza op��o selecionada
};

#endif