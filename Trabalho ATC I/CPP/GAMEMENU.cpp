// game_menu.h
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

// game_menu.cpp
#include "game_menu.h"

GameMenu::GameMenu(const std::string& menuTitle)
    : title(menuTitle),
    selectedOption(0),
    isActive(false),
    showHelp(true),
    startX(5),
    startY(3),
    width(70),
    height(20)
{
    // Configura menu principal
    showMainMenu();
}

void GameMenu::showMainMenu() {
    menuItems.clear();
    addMenuItem("Novo Jogo", "Inicia uma nova partida do Pac-Man",
        []() { /* Lógica de novo jogo */ });

    addMenuItem("Continuar", "Retorna ao jogo em andamento",
        []() { /* Lógica de continuar */ });

    addMenuItem("Controles", "Mostra os controles do jogo",
        [this]() { showControlsMenu(); });

    addMenuItem("Configurações", "Ajusta configurações do jogo",
        [this]() { showConfigMenu(); });

    addMenuItem("Pontuações", "Mostra as melhores pontuações",
        []() { /* Lógica de pontuações */ });

    addMenuItem("Sair", "Sair do jogo (salva progresso)",
        [this]() { showConfirmExit(); });
}

void GameMenu::showPauseMenu() {
    menuItems.clear();
    addMenuItem("Continuar", "Retorna ao jogo",
        []() { /* Lógica de continuar */ });

    addMenuItem("Reiniciar Nível", "Recomeça o nível atual",
        []() { /* Lógica de reiniciar */ });

    addMenuItem("Configurações", "Ajusta configurações do jogo",
        [this]() { showConfigMenu(); });

    addMenuItem("Menu Principal", "Volta ao menu principal",
        [this]() { showMainMenu(); });
}

void GameMenu::showConfigMenu() {
    menuItems.clear();
    addMenuItem("Dificuldade", "Ajusta a dificuldade do jogo",
        []() { /* Lógica de dificuldade */ });

    addMenuItem("Som: Ligado", "Liga/Desliga efeitos sonoros",
        []() { /* Lógica de som */ });

    addMenuItem("Música: Ligada", "Liga/Desliga música de fundo",
        []() { /* Lógica de música */ });

    addMenuItem("Voltar", "Retorna ao menu anterior",
        [this]() { showMainMenu(); });
}

void GameMenu::showControlsMenu() {
    menuItems.clear();
    addMenuItem("Movimento", "Setas direcionais - Move o Pac-Man",
        nullptr, false);

    addMenuItem("Pausa", "P - Pausa o jogo",
        nullptr, false);

    addMenuItem("Power Pellet", "Quando ativo, permite comer fantasmas",
        nullptr, false);

    addMenuItem("Voltar", "Retorna ao menu anterior",
        [this]() { showMainMenu(); });
}

void GameMenu::showConfirmExit() {
    menuItems.clear();
    addMenuItem("Sim", "Confirma e sai do jogo",
        []() { endwin(); exit(0); });

    addMenuItem("Não", "Retorna ao menu anterior",
        [this]() { showMainMenu(); });
}

void GameMenu::display() {
    clearMenuArea();
    drawFrame();
    drawTitle();
    drawOptions();
    if (showHelp) {
        drawDescription();
    }
    drawControls();
    refresh();
}

void GameMenu::drawFrame() {
    // Desenha borda do menu
    for (int y = startY; y < startY + height; y++) {
        mvaddch(y, startX, ACS_VLINE);
        mvaddch(y, startX + width, ACS_VLINE);
    }

    for (int x = startX; x < startX + width; x++) {
        mvaddch(startY, x, ACS_HLINE);
        mvaddch(startY + height, x, ACS_HLINE);
    }

    // Cantos
    mvaddch(startY, startX, ACS_ULCORNER);
    mvaddch(startY, startX + width, ACS_URCORNER);
    mvaddch(startY + height, startX, ACS_LLCORNER);
    mvaddch(startY + height, startX + width, ACS_LRCORNER);
}

void GameMenu::drawTitle() {
    attron(A_BOLD | COLOR_PAIR(6));
    mvprintw(startY + 1, startX + (width - title.length()) / 2, "%s", title.c_str());
    attroff(A_BOLD | COLOR_PAIR(6));
}

void GameMenu::drawOptions() {
    int y = startY + 3;
    for (size_t i = 0; i < menuItems.size(); i++) {
        if (i == selectedOption) {
            attron(A_REVERSE);
        }
        if (!menuItems[i].isEnabled) {
            attron(COLOR_PAIR(8)); // Cor para opções desabilitadas
        }

        mvprintw(y, startX + 2, "%s", menuItems[i].label.c_str());

        if (!menuItems[i].isEnabled) {
            attroff(COLOR_PAIR(8));
        }
        if (i == selectedOption) {
            attroff(A_REVERSE);
        }
        y += 2;
    }
}

void GameMenu::drawDescription() {
    if (selectedOption >= 0 && selectedOption < static_cast<int>(menuItems.size())) {
        const auto& item = menuItems[selectedOption];
        if (!item.description.empty()) {
            attron(COLOR_PAIR(6));
            mvprintw(startY + height - 3, startX + 2, "%s", item.description.c_str());
            attroff(COLOR_PAIR(6));
        }
    }
}

void GameMenu::drawControls() {
    attron(COLOR_PAIR(6));
    mvprintw(startY + height - 1, startX + 2,
        "↑↓: Selecionar   Enter: Confirmar   Esc: Voltar");
    attroff(COLOR_PAIR(6));
}

void GameMenu::handleInput(int key) {
    switch (key) {
    case KEY_UP:
        moveSelection(MenuNavigation::UP);
        break;
    case KEY_DOWN:
        moveSelection(MenuNavigation::DOWN);
        break;
    case 10: // Enter
        selectCurrentOption();
        break;
    case 27: // Esc
        if (title != "Menu Principal") {
            showMainMenu();
        }
        break;
    }
    display();
}

void GameMenu::moveSelection(MenuNavigation dir) {
    switch (dir) {
    case MenuNavigation::UP:
        do {
            selectedOption = (selectedOption - 1 + menuItems.size()) % menuItems.size();
        } while (!menuItems[selectedOption].isEnabled);
        break;

    case MenuNavigation::DOWN:
        do {
            selectedOption = (selectedOption + 1) % menuItems.size();
        } while (!menuItems[selectedOption].isEnabled);
        break;

    default:
        break;
    }
}

void GameMenu::selectCurrentOption() {
    if (selectedOption >= 0 && selectedOption < static_cast<int>(menuItems.size())) {
        const auto& item = menuItems[selectedOption];
        if (item.isEnabled && item.action) {
            item.action();
        }
    }
}

void GameMenu::clearMenuArea() {
    for (int y = startY; y < startY + height + 1; y++) {
        move(y, startX);
        clrtoeol();
    }
}

void GameMenu::addMenuItem(const std::string& label,
    const std::string& description,
    std::function<void()> action,
    bool enabled) {
    menuItems.emplace_back(label, description, action, enabled);
}