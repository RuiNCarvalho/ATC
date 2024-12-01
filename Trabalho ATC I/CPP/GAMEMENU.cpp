#include "game_menu.h"
#include <cmath>

// ---- Implementa��o da classe Button (Bot�o) ----

// Construtor do bot�o: inicializa um novo bot�o com posi��o e texto
Button::Button(int x, int y, const std::string& buttonLabel)
    : GameObject(x, y, 100, 30, 0),  // Cria um GameObject com largura=100, altura=30, velocidade=0
    label(buttonLabel),            // Define o texto do bot�o
    isSelected(false) {            // Come�a n�o selecionado
}

// Bot�es n�o se movem, ent�o deixamos vazio
void Button::move() {
    // N�o faz nada
}

// O desenho ser� feito na classe PacmanUI
void Button::draw() {
    // Implementa��o ser� feita em outro lugar
}

// Verifica se um clique do mouse (posi��o x,y) est� dentro do bot�o
bool Button::isClicked(int mouseX, int mouseY) {
    return mouseX >= getX() && mouseX <= getX() + getWidth() &&
        mouseY >= getY() && mouseY <= getY() + getHeight();
}

// Define se o bot�o est� selecionado ou n�o
void Button::setSelected(bool selected) {
    isSelected = selected;
}

// Retorna se o bot�o est� selecionado
bool Button::getSelected() const {
    return isSelected;
}

// Retorna o texto do bot�o
std::string Button::getLabel() const {
    return label;
}

// ---- Implementa��o da classe MenuOption (Op��o do Menu) ----

// Construtor: cria uma nova op��o de menu
MenuOption::MenuOption(int x, int y, const std::string& label, std::function<void()> actionFunc)
    : Button(x, y, label),    // Cria o bot�o com posi��o e texto
    action(actionFunc) {     // Guarda a fun��o que ser� executada quando selecionado
}

// Executa a a��o associada a esta op��o do menu
void MenuOption::executeAction() {
    if (action) {  // Se tiver uma a��o definida
        action();  // Executa a a��o
    }
}

// ---- Implementa��o da classe GameMenu (Menu do Jogo) ----

// Construtor: cria um novo menu
GameMenu::GameMenu(const std::string& menuTitle)
    : title(menuTitle),      // Define o t�tulo
    selectedOption(0),     // Primeira op��o come�a selecionada
    isActive(false) {      // Menu come�a inativo
}

// Destrutor: limpa a mem�ria quando o menu � destru�do
GameMenu::~GameMenu() {
    // Deleta todas as op��es do menu
    for (auto* option : options) {
        delete option;
    }
    options.clear();  // Limpa a lista
}

// Adiciona uma nova op��o ao menu
void GameMenu::addOption(const std::string& label, int x, int y, std::function<void()> action) {
    options.push_back(new MenuOption(x, y, label, action));
    // Se for a primeira op��o, seleciona ela
    if (options.size() == 1) {
        options[0]->setSelected(true);
    }
}

// Mostra o menu
void GameMenu::displayMenu() {
    isActive = true;
    updateSelectedOption();
}

// Controla a navega��o no menu (responde �s teclas)
void GameMenu::navigateMenu(MenuNavigation direction) {
    if (!isActive) return;  // Se o menu n�o estiver ativo, n�o faz nada

    switch (direction) {
    case MenuNavigation::UP:  // Se apertou para cima
        // Move para a op��o anterior (volta para o final se estiver no in�cio)
        selectedOption = (selectedOption - 1 + options.size()) % options.size();
        break;
    case MenuNavigation::DOWN:  // Se apertou para baixo
        // Move para a pr�xima op��o (volta para o in�cio se estiver no final)
        selectedOption = (selectedOption + 1) % options.size();
        break;
    case MenuNavigation::ENTER:  // Se apertou Enter
        selectOption();          // Seleciona a op��o atual
        break;
    case MenuNavigation::ESCAPE:  // Se apertou Esc
        exitMenu();               // Fecha o menu
        break;
    }
    updateSelectedOption();  // Atualiza qual op��o est� selecionada
}

// Seleciona a op��o atual do menu
void GameMenu::selectOption() {
    if (isActive && selectedOption >= 0 && selectedOption < options.size()) {
        options[selectedOption]->executeAction();  // Executa a a��o da op��o selecionada
    }
}

// Fecha o menu
void GameMenu::exitMenu() {
    isActive = false;
}

// Atualiza qual op��o est� selecionada (visual)
void GameMenu::updateSelectedOption() {
    for (size_t i = 0; i < options.size(); i++) {
        options[i]->setSelected(i == selectedOption);
    }
}

// Retorna se o menu est� ativo
bool GameMenu::isMenuActive() const {
    return isActive;
}

// Retorna o t�tulo do menu
std::string GameMenu::getTitle() const {
    return title;
}

// Retorna a lista de op��es do menu
const std::vector<MenuOption*>& GameMenu::getOptions() const {
    return options;
}

// Retorna qual op��o est� selecionada
int GameMenu::getSelectedIndex() const {
    return selectedOption;
}