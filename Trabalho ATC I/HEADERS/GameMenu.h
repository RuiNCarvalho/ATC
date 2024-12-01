#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <string>
#include <vector>
#include <functional>
#include "game_object.h"

// Define as direções que podemos usar para navegar no menu
enum class MenuNavigation {
    UP,     // Mover para cima
    DOWN,   // Mover para baixo
    ENTER,  // Selecionar opção
    ESCAPE  // Sair do menu
};

// Classe Button (Botão) - É como um modelo para criar botões no menu
// Herda de GameObject para ter posição na tela (x,y) e tamanho
class Button : public GameObject {
private:
    // Informações que cada botão tem
    std::string label;    // Texto que aparece no botão (exemplo: "Novo Jogo")
    bool isSelected;      // Indica se o botão está selecionado ou não

public:
    // Construtor: função chamada quando criamos um novo botão
    Button(int x, int y, const std::string& buttonLabel);

    // Destrutor virtual: permite que as classes que herdam de Button
    // sejam deletadas corretamente
    virtual ~Button() = default;


//................................................................//
    // Funções que precisamos implementar por causa do GameObject
    void move() override;    // Como o botão se move (neste caso, não move)
   
  //...............................................................//

    // Funções específicas para botões
    virtual bool isClicked(int mouseX, int mouseY);  // Verifica se o mouse clicou no botão
    void setSelected(bool selected);                 // Marca o botão como selecionado ou não
    bool getSelected() const;                        // Retorna se o botão está selecionado
    std::string getLabel() const;                    // Retorna o texto do botão
};

// Classe MenuOption (Opção do Menu) - É um tipo especial de botão
// que também guarda uma ação para executar quando clicado
class MenuOption : public Button {
private:
    // Função que será executada quando clicarmos nesta opção
    std::function<void()> action;

public:
    // Construtor: cria uma nova opção do menu
    MenuOption(int x, int y, const std::string& label, std::function<void()> actionFunc);

    // Executa a ação associada a esta opção
    void executeAction();
};

// Classe GameMenu - Controla todo o menu do jogo
class GameMenu {
private:
    std::string title;                    // Título do menu
    std::vector<MenuOption*> options;     // Lista de opções do menu
    int selectedOption;                   // Qual opção está selecionada
    bool isActive;                        // Se o menu está ativo ou não

    // Função interna para atualizar qual opção está selecionada
    void updateSelectedOption();

public:
    // Construtor: cria um novo menu com um título
    GameMenu(const std::string& menuTitle);

    // Destrutor: limpa a memória quando o menu é destruído
    ~GameMenu();

    // Impede que o menu seja copiado (evita problemas com memória)
    GameMenu(const GameMenu&) = delete;
    GameMenu& operator=(const GameMenu&) = delete;

    // Funções principais do menu
    // Adiciona uma nova opção ao menu
    void addOption(const std::string& label, int x, int y, std::function<void()> action);

    // Mostra o menu na tela
    void displayMenu();

    // Move a seleção no menu (responde às teclas)
    void navigateMenu(MenuNavigation direction);

    // Seleciona a opção atual
    void selectOption();

    // Fecha o menu
    void exitMenu();

    // Funções para obter informações do menu
    bool isMenuActive() const;                           // Retorna se o menu está ativo
    std::string getTitle() const;                       // Retorna o título do menu
    const std::vector<MenuOption*>& getOptions() const; // Retorna a lista de opções
    int getSelectedIndex() const;                       // Retorna qual opção está selecionada
};

#endif // GAME_MENU_H
