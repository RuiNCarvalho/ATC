#include "game_menu.h"
#include <cmath>

// ---- Implementação da classe Button (Botão) ----

// Construtor do botão: inicializa um novo botão com posição e texto
Button::Button(int x, int y, const std::string& buttonLabel)
    : GameObject(x, y, 100, 30, 0),  // Cria um GameObject com largura=100, altura=30, velocidade=0
    label(buttonLabel),            // Define o texto do botão
    isSelected(false) {            // Começa não selecionado
}

// Botões não se movem, então deixamos vazio
void Button::move() {
    // Não faz nada
}

// O desenho será feito na classe PacmanUI
void Button::draw() {
    // Implementação será feita em outro lugar
}

// Verifica se um clique do mouse (posição x,y) está dentro do botão
bool Button::isClicked(int mouseX, int mouseY) {
    return mouseX >= getX() && mouseX <= getX() + getWidth() &&
        mouseY >= getY() && mouseY <= getY() + getHeight();
}

// Define se o botão está selecionado ou não
void Button::setSelected(bool selected) {
    isSelected = selected;
}

// Retorna se o botão está selecionado
bool Button::getSelected() const {
    return isSelected;
}

// Retorna o texto do botão
std::string Button::getLabel() const {
    return label;
}

// ---- Implementação da classe MenuOption (Opção do Menu) ----

// Construtor: cria uma nova opção de menu
MenuOption::MenuOption(int x, int y, const std::string& label, std::function<void()> actionFunc)
    : Button(x, y, label),    // Cria o botão com posição e texto
    action(actionFunc) {     // Guarda a função que será executada quando selecionado
}

// Executa a ação associada a esta opção do menu
void MenuOption::executeAction() {
    if (action) {  // Se tiver uma ação definida
        action();  // Executa a ação
    }
}

// ---- Implementação da classe GameMenu (Menu do Jogo) ----

// Construtor: cria um novo menu
GameMenu::GameMenu(const std::string& menuTitle)
    : title(menuTitle),      // Define o título
    selectedOption(0),     // Primeira opção começa selecionada
    isActive(false) {      // Menu começa inativo
}

// Destrutor: limpa a memória quando o menu é destruído
GameMenu::~GameMenu() {
    // Deleta todas as opções do menu
    for (auto* option : options) {
        delete option;
    }
    options.clear();  // Limpa a lista
}

// Adiciona uma nova opção ao menu
void GameMenu::addOption(const std::string& label, int x, int y, std::function<void()> action) {
    options.push_back(new MenuOption(x, y, label, action));
    // Se for a primeira opção, seleciona ela
    if (options.size() == 1) {
        options[0]->setSelected(true);
    }
}

// Mostra o menu
void GameMenu::displayMenu() {
    isActive = true;
    updateSelectedOption();
}

// Controla a navegação no menu (responde às teclas)
void GameMenu::navigateMenu(MenuNavigation direction) {
    if (!isActive) return;  // Se o menu não estiver ativo, não faz nada

    switch (direction) {
    case MenuNavigation::UP:  // Se apertou para cima
        // Move para a opção anterior (volta para o final se estiver no início)
        selectedOption = (selectedOption - 1 + options.size()) % options.size();
        break;
    case MenuNavigation::DOWN:  // Se apertou para baixo
        // Move para a próxima opção (volta para o início se estiver no final)
        selectedOption = (selectedOption + 1) % options.size();
        break;
    case MenuNavigation::ENTER:  // Se apertou Enter
        selectOption();          // Seleciona a opção atual
        break;
    case MenuNavigation::ESCAPE:  // Se apertou Esc
        exitMenu();               // Fecha o menu
        break;
    }
    updateSelectedOption();  // Atualiza qual opção está selecionada
}

// Seleciona a opção atual do menu
void GameMenu::selectOption() {
    if (isActive && selectedOption >= 0 && selectedOption < options.size()) {
        options[selectedOption]->executeAction();  // Executa a ação da opção selecionada
    }
}

// Fecha o menu
void GameMenu::exitMenu() {
    isActive = false;
}

// Atualiza qual opção está selecionada (visual)
void GameMenu::updateSelectedOption() {
    for (size_t i = 0; i < options.size(); i++) {
        options[i]->setSelected(i == selectedOption);
    }
}

// Retorna se o menu está ativo
bool GameMenu::isMenuActive() const {
    return isActive;
}

// Retorna o título do menu
std::string GameMenu::getTitle() const {
    return title;
}

// Retorna a lista de opções do menu
const std::vector<MenuOption*>& GameMenu::getOptions() const {
    return options;
}

// Retorna qual opção está selecionada
int GameMenu::getSelectedIndex() const {
    return selectedOption;
}