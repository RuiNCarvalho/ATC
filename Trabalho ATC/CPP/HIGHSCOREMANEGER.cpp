#include "highscore_manager.h"
#include <curses.h>
#include <iostream>

// Construtor padrão: quando o HighScoreManager é criado, carrega as pontuações salvas
HighScoreManager::HighScoreManager() {
    // Ao criar o gerenciador, tentamos carregar pontuações salvas anteriormente
    loadScores();
}

// Método para adicionar uma nova pontuação ao registro
void HighScoreManager::addScore(const std::string& playerName, int score) {
    // Cria uma nova entrada de pontuação com o nome e score do jogador
    ScoreEntry newEntry(playerName, score);
    // Adiciona a nova entrada à lista de pontuações
    scores.push_back(newEntry);
    // Ordena as pontuações para manter sempre a lista organizada
    sortScores();
    // Limita o número de pontuações para evitar lista muito grande
    limitScores();
}

// Método para obter as melhores pontuações
std::list<ScoreEntry> HighScoreManager::getTopScores(size_t count) {
    // Se o número de pontuações for menor que o solicitado, retorna todas
    if (count > scores.size()) {
        count = scores.size();
    }
    // Cria uma lista temporária com as melhores pontuações
    std::list<ScoreEntry> topScores;
    auto it = scores.begin();
    // Adiciona as primeiras "count" pontuações na lista
    for (size_t i = 0; i < count && it != scores.end(); ++i, ++it) {
        topScores.push_back(*it);
    }
    return topScores;
}

// Método para salvar pontuações em arquivo
void HighScoreManager::saveScores() {
    // Abre o arquivo para escrita, substituindo conteúdo anterior
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
    // Salva cada pontuação no arquivo (nome;pontuação)
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

// Método para carregar pontuações de arquivo
void HighScoreManager::loadScores() {
    // Limpa pontuações atuais antes de carregar
    scores.clear();
    // Abre arquivo para leitura
    std::ifstream file(HIGHSCORE_FILE);
    // Se arquivo não existe, não faz nada
    if (!file.is_open()) {
        return;
    }
    std::string line;
    // Lê linha por linha do arquivo
    while (std::getline(file, line)) {
        // Encontra a posição do separador
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
    // Organiza pontuações após carregar
    sortScores();
    limitScores();
}

// Método privado para ordenar pontuações
void HighScoreManager::sortScores() {
    // Usa o método sort da STL com o operador '>' definido em ScoreEntry
    // Isso ordena do maior para o menor
    scores.sort(std::greater<ScoreEntry>());
}

// Método privado para limitar número de pontuações
void HighScoreManager::limitScores(size_t maxScores) {
    // Se lista maior que máximo, remove últimas entradas
    while (scores.size() > maxScores) {
        scores.pop_back();
    }
}