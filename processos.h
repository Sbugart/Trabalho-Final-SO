#ifndef _processos_h_
#define _processos_h_

#include <bits/stdc++.h>
using namespace std;

/* Váriavel global que armazena a quantidade espaço
de memória que será usada para armazenar as mensagens.*/
const int MEM_LENGTH = 100;

/**
 * @brief Definição da struct processos
 * @param id Identificador criado para o processo.
 * @param duracao Tempo de duração do proceso.
 * @param start_time Tempo de início do processo.
 * @param end_time Tempo de finalização do processo.
 * @param estado Estado do processo no processador.
 * @param pipe_menssenger Pipe que guarda o processo remetente das mensagens.
 * @param pipe_message Pipe que guarda as mensagens para o respectivo processo.
 */
struct processos{
    int id;
    int duracao;
    int start_time;
    int end_time;
    string estado;
    queue<int> pipe_messenger;
    queue<string> pipe_message;

    processos(int duracao, int inicio) : duracao(duracao), start_time(inicio), estado("Novo") {};
};

/**
 * @brief Função responsável pela criação dos processos.
 * @details Cria os processos.
 * @param duracao Tempo de duração do processo.
 * @param inicio Tempo de início do processo.
 * @return Retorna um novo processo.
 */
processos cria_process(int duracao, int inicio);

/**
 * @brief Inicializa o programa.
 * @details Define tempos de duração e início aleatórios para os processos e os organiza.
 * @param quantidade_processos Quantidade de processos do programa.
 * @return Retorna o programa a ser escalonado.
 */
vector<processos> inicia_programa(int quantidade_processos);

#endif 
