#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <string>
#include <vector>
#include <functional>
#include <curses.h>
#include "game_object.h"

// Define as direções possíveis no menu
enum class MenuNavigation {
    UP,     // Seta para cima
    DOWN,   // Seta para baixo
    ENTER,  // Tecla Enter
    ESCAPE  // Tecla Esc
};

// Estrutura para representar cada opção do menu
struct MenuItem {
    std::string label;           // Texto da opção
    std::string description;     // Descrição/explicação da opção
    std::function<void()> action;// Função a ser executada
    bool isEnabled;              // Se a opção está habilitada

    MenuItem(const std::string& lbl, const std::string& desc,
        std::function<void()> act, bool enabled = true)
        : label(lbl), description(desc), action(act), isEnabled(enabled) {}
};

class GameMenu {
private:
    std::string title;                    // Título do menu
    std::vector<MenuItem> menuItems;      // Lista de opções do menu
    int selectedOption;                   // Índice da opção selecionada
    bool isActive;                        // Se o menu está ativo
    bool showHelp;                        // Se mostra descrições das opções

    // Dimensões e posição do menu
    int startX;
    int startY;
    int width;
    int height;

public:
    // Construtor: inicializa o menu
    GameMenu(const std::string& menuTitle);

    // Adiciona uma nova opção ao menu
    void addMenuItem(const std::string& label,
        const std::string& description,
        std::function<void()> action,
        bool enabled = true);

    // Funções principais do menu
    void display();                           // Mostra o menu na tela
    void handleInput(int key);                // Processa entrada do usuário
    void selectCurrentOption();               // Executa opção selecionada

    // Menus específicos
    void showMainMenu();                      // Menu principal
    void showPauseMenu();                     // Menu de pausa
    void showConfigMenu();                    // Menu de configurações
    void showControlsMenu();                  // Menu de controles
    void showConfirmExit();                   // Confirmação de saída

private:
    // Funções auxiliares de desenho
    void drawFrame();                         // Desenha borda do menu
    void drawTitle();                         // Desenha título
    void drawOptions();                       // Desenha opções
    void drawDescription();                   // Desenha descrição da opção atual
    void drawControls();                      // Desenha teclas de controle
    void clearMenuArea();                     // Limpa área do menu

    // Funções de navegação
    void moveSelection(MenuNavigation dir);   // Move seleção
    void updateSelection();                   // Atualiza opção selecionada
};

#endif