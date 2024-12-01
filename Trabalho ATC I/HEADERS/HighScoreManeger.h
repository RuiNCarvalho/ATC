#ifndef HIGHSCORE_MANAGER_H
#define HIGHSCORE_MANAGER_H

#include <list>     // Lista para armazenar coleção de pontuações
#include <string>   // Para manipular texto (nomes dos jogadores)
#include <fstream>  // Para operações de arquivo (salvar/carregar)
#include <algorithm>// Para realizar operações como ordenação

// Estrutura que representa uma entrada de pontuação
// Pense nisso como uma "ficha" que guarda informações de um jogador
struct ScoreEntry {
    std::string playerName;  // Nome do jogador
    int score;               // Pontuação feita

    // Construtor que permite criar uma entrada vazia ou com dados
    // Valores padrão permitem criar uma entrada sem passar argumentos
    ScoreEntry(const std::string& name = "", int playerScore = 0)
        : playerName(name), score(playerScore) {}

    // Operador de comparação para permitir ordenação das pontuações
    // Ajuda a organizar as pontuações do maior para o menor
    bool operator>(const ScoreEntry& other) const {
        return score > other.score;
    }
};

// Classe responsável por gerenciar as pontuações altas
class HighScoreManager {
private:
    // Lista que armazena todas as pontuações
    // Como um caderno de registro de pontuações
    std::list<ScoreEntry> scores;

    // Caminho do arquivo onde as pontuações serão salvas
    // Permite manter as pontuações mesmo depois de fechar o jogo
    const std::string HIGHSCORE_FILE = "highscores.txt";

public:
    // Construtor padrão
    // Pode ser usado para inicializar qualquer recurso necessário
    HighScoreManager();

    // Adiciona uma nova pontuação ao registro
    void addScore(const std::string& playerName, int score);

    // Obtém as melhores pontuações
    // @return Lista das melhores pontuações (padrão: top 10)
    std::list<ScoreEntry> getTopScores(size_t count = 10);

    // Salva as pontuações em um arquivo
    // Permite preservar o histórico de pontuações
    void saveScores();

    // Carrega pontuações de um arquivo
    // Recupera pontuações salvas anteriormente
    void loadScores();

private:
    // Ordena as pontuações do maior para o menor
    // Usa o operador de comparação que criamos na estrutura
    void sortScores();

    // Limita o número máximo de pontuações armazenadas
    // Evita que a lista cresça indefinidamente
    void limitScores(size_t maxScores = 10);
};

#endif
