#include "highscore_manager.h"
#include <curses.h>
#include <iostream>

// Construtor padr�o: quando o HighScoreManager � criado, carrega as pontua��es salvas
HighScoreManager::HighScoreManager() {
    // Ao criar o gerenciador, tentamos carregar pontua��es salvas anteriormente
    loadScores();
}

// M�todo para adicionar uma nova pontua��o ao registro
void HighScoreManager::addScore(const std::string& playerName, int score) {
    // Cria uma nova entrada de pontua��o com o nome e score do jogador
    ScoreEntry newEntry(playerName, score);
    // Adiciona a nova entrada � lista de pontua��es
    scores.push_back(newEntry);
    // Ordena as pontua��es para manter sempre a lista organizada
    sortScores();
    // Limita o n�mero de pontua��es para evitar lista muito grande
    limitScores();
}

// M�todo para obter as melhores pontua��es
std::list<ScoreEntry> HighScoreManager::getTopScores(size_t count) {
    // Se o n�mero de pontua��es for menor que o solicitado, retorna todas
    if (count > scores.size()) {
        count = scores.size();
    }
    // Cria uma lista tempor�ria com as melhores pontua��es
    std::list<ScoreEntry> topScores;
    auto it = scores.begin();
    // Adiciona as primeiras "count" pontua��es na lista
    for (size_t i = 0; i < count && it != scores.end(); ++i, ++it) {
        topScores.push_back(*it);
    }
    return topScores;
}

// M�todo para salvar pontua��es em arquivo
void HighScoreManager::saveScores() {
    // Abre o arquivo para escrita, substituindo conte�do anterior
    std::ofstream file(HIGHSCORE_FILE);
    // Verifica se o arquivo foi aberto corretamente
    if (!file.is_open()) {
        // Usa PDCurses para mostrar mensagem de erro
        clear();
        mvprintw(10, 10, "Erro: Nao foi possivel abrir o arquivo de pontuacoes.");
        mvprintw(11, 10, "Pressione qualquer tecla para continuar...");
        refresh();
        getch(); // Espera uma tecla ser pressionada
        return;
    }
    // Salva cada pontua��o no arquivo (nome;pontua��o)
    for (const auto& entry : scores) {
        file << entry.playerName << ";" << entry.score << std::endl;
    }
    file.close();

    // Mostra mensagem de sucesso usando PDCurses
    clear();
    mvprintw(10, 10, "Pontuacoes salvas com sucesso!");
    mvprintw(11, 10, "Pressione qualquer tecla para continuar...");
    refresh();
    getch(); // Espera uma tecla ser pressionada
}

// M�todo para carregar pontua��es de arquivo
void HighScoreManager::loadScores() {
    // Limpa pontua��es atuais antes de carregar
    scores.clear();
    // Abre arquivo para leitura
    std::ifstream file(HIGHSCORE_FILE);
    // Se arquivo n�o existe, n�o faz nada
    if (!file.is_open()) {
        return;
    }
    std::string line;
    // L� linha por linha do arquivo
    while (std::getline(file, line)) {
        // Encontra a posi��o do separador
        size_t separatorPos = line.find(';');
        // Se linha estiver no formato correto
        if (separatorPos != std::string::npos) {
            std::string playerName = line.substr(0, separatorPos);
            int score = std::stoi(line.substr(separatorPos + 1));
            // Adiciona entrada ao registro
            scores.push_back(ScoreEntry(playerName, score));
        }
    }
    file.close();
    // Organiza pontua��es ap�s carregar
    sortScores();
    limitScores();
}

// M�todo privado para ordenar pontua��es
void HighScoreManager::sortScores() {
    // Usa o m�todo sort da STL com o operador '>' definido em ScoreEntry
    // Isso ordena do maior para o menor
    scores.sort(std::greater<ScoreEntry>());
}

// M�todo privado para limitar n�mero de pontua��es
void HighScoreManager::limitScores(size_t maxScores) {
    // Se lista maior que m�ximo, remove �ltimas entradas
    while (scores.size() > maxScores) {
        scores.pop_back();
    }
}