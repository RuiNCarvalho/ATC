#ifndef HIGHSCORE_MANAGER_H
#define HIGHSCORE_MANAGER_H

#include <list>     // Lista para armazenar cole��o de pontua��es
#include <string>   // Para manipular texto (nomes dos jogadores)
#include <fstream>  // Para opera��es de arquivo (salvar/carregar)
#include <algorithm>// Para realizar opera��es como ordena��o

// Estrutura que representa uma entrada de pontua��o
// Pense nisso como uma "ficha" que guarda informa��es de um jogador
struct ScoreEntry {
    std::string playerName;  // Nome do jogador
    int score;               // Pontua��o feita

    // Construtor que permite criar uma entrada vazia ou com dados
    // Valores padr�o permitem criar uma entrada sem passar argumentos
    ScoreEntry(const std::string& name = "", int playerScore = 0)
        : playerName(name), score(playerScore) {}

    // Operador de compara��o para permitir ordena��o das pontua��es
    // Ajuda a organizar as pontua��es do maior para o menor
    bool operator>(const ScoreEntry& other) const {
        return score > other.score;
    }
};

// Classe respons�vel por gerenciar as pontua��es altas
class HighScoreManager {
private:
    // Lista que armazena todas as pontua��es
    // Como um caderno de registro de pontua��es
    std::list<ScoreEntry> scores;

    // Caminho do arquivo onde as pontua��es ser�o salvas
    // Permite manter as pontua��es mesmo depois de fechar o jogo
    const std::string HIGHSCORE_FILE = "highscores.txt";

public:
    // Construtor padr�o
    // Pode ser usado para inicializar qualquer recurso necess�rio
    HighScoreManager();

    // Adiciona uma nova pontua��o ao registro
    void addScore(const std::string& playerName, int score);

    // Obt�m as melhores pontua��es
    // @return Lista das melhores pontua��es (padr�o: top 10)
    std::list<ScoreEntry> getTopScores(size_t count = 10);

    // Salva as pontua��es em um arquivo
    // Permite preservar o hist�rico de pontua��es
    void saveScores();

    // Carrega pontua��es de um arquivo
    // Recupera pontua��es salvas anteriormente
    void loadScores();

private:
    // Ordena as pontua��es do maior para o menor
    // Usa o operador de compara��o que criamos na estrutura
    void sortScores();

    // Limita o n�mero m�ximo de pontua��es armazenadas
    // Evita que a lista cres�a indefinidamente
    void limitScores(size_t maxScores = 10);
};

#endif
