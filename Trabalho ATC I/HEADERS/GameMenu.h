#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <string>
#include <vector>
#include <functional>
#include "game_object.h"

// Define as dire��es que podemos usar para navegar no menu
enum class MenuNavigation {
    UP,     // Mover para cima
    DOWN,   // Mover para baixo
    ENTER,  // Selecionar op��o
    ESCAPE  // Sair do menu
};

// Classe Button (Bot�o) - � como um modelo para criar bot�es no menu
// Herda de GameObject para ter posi��o na tela (x,y) e tamanho
class Button : public GameObject {
private:
    // Informa��es que cada bot�o tem
    std::string label;    // Texto que aparece no bot�o (exemplo: "Novo Jogo")
    bool isSelected;      // Indica se o bot�o est� selecionado ou n�o

public:
    // Construtor: fun��o chamada quando criamos um novo bot�o
    Button(int x, int y, const std::string& buttonLabel);

    // Destrutor virtual: permite que as classes que herdam de Button
    // sejam deletadas corretamente
    virtual ~Button() = default;


//................................................................//
    // Fun��es que precisamos implementar por causa do GameObject
    void move() override;    // Como o bot�o se move (neste caso, n�o move)
   
  //...............................................................//

    // Fun��es espec�ficas para bot�es
    virtual bool isClicked(int mouseX, int mouseY);  // Verifica se o mouse clicou no bot�o
    void setSelected(bool selected);                 // Marca o bot�o como selecionado ou n�o
    bool getSelected() const;                        // Retorna se o bot�o est� selecionado
    std::string getLabel() const;                    // Retorna o texto do bot�o
};

// Classe MenuOption (Op��o do Menu) - � um tipo especial de bot�o
// que tamb�m guarda uma a��o para executar quando clicado
class MenuOption : public Button {
private:
    // Fun��o que ser� executada quando clicarmos nesta op��o
    std::function<void()> action;

public:
    // Construtor: cria uma nova op��o do menu
    MenuOption(int x, int y, const std::string& label, std::function<void()> actionFunc);

    // Executa a a��o associada a esta op��o
    void executeAction();
};

// Classe GameMenu - Controla todo o menu do jogo
class GameMenu {
private:
    std::string title;                    // T�tulo do menu
    std::vector<MenuOption*> options;     // Lista de op��es do menu
    int selectedOption;                   // Qual op��o est� selecionada
    bool isActive;                        // Se o menu est� ativo ou n�o

    // Fun��o interna para atualizar qual op��o est� selecionada
    void updateSelectedOption();

public:
    // Construtor: cria um novo menu com um t�tulo
    GameMenu(const std::string& menuTitle);

    // Destrutor: limpa a mem�ria quando o menu � destru�do
    ~GameMenu();

    // Impede que o menu seja copiado (evita problemas com mem�ria)
    GameMenu(const GameMenu&) = delete;
    GameMenu& operator=(const GameMenu&) = delete;

    // Fun��es principais do menu
    // Adiciona uma nova op��o ao menu
    void addOption(const std::string& label, int x, int y, std::function<void()> action);

    // Mostra o menu na tela
    void displayMenu();

    // Move a sele��o no menu (responde �s teclas)
    void navigateMenu(MenuNavigation direction);

    // Seleciona a op��o atual
    void selectOption();

    // Fecha o menu
    void exitMenu();

    // Fun��es para obter informa��es do menu
    bool isMenuActive() const;                           // Retorna se o menu est� ativo
    std::string getTitle() const;                       // Retorna o t�tulo do menu
    const std::vector<MenuOption*>& getOptions() const; // Retorna a lista de op��es
    int getSelectedIndex() const;                       // Retorna qual op��o est� selecionada
};

#endif // GAME_MENU_H
