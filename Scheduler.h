#ifndef _Scheduler_h_
#define _Scheduler_h_

#include "Process_manager.h"

/**
 * @brief Definição da struct escalonador.
 * @param fila Fila de processos que serão executados.
 * @param memoria_mensagens Espaço de memória fixo para as mensagens dos processos.
 * @param usado Informa se aquele espaço do memoria_mensagem armazena um lixo ou não (0 se for lixo de mem).
 * @param receber Armazena o Id do processo que recebeu uma informação.
 * @param wt Vetor que guarda o tempo de espera de cada processo.
 * @param tat Vetor que guarda o tempo total de execução.
 * @param tempo_restante Vetor que guarda o tempo restante de cada processo.
 * @attention Na função de inicialização, além de inicializar os vetores, o tempo_restante já recebe os seus respectivos dados.
 */
struct escalonador
{
    queue<processos*> fila;
    vector<string> memoria_mensagens;
    vector<int> usado, receber;
    vector<int> wt, tat, tempo_restante;

    escalonador(int tamanho, vector<processos> &programa) : memoria_mensagens(MEM_LENGTH), usado(MEM_LENGTH, 0),
    receber(MEM_LENGTH), wt(tamanho, 0), tat(tamanho, 0), tempo_restante(tamanho, 0) {
        transform(programa.begin(), programa.end(), tempo_restante.begin(),
        [](const processos &p){return p.duracao;});
    };
};

/**
 * @brief Aplicação do escalonador Round Robin.
 * @param programa Lista de processos do programa.
 * @param quantum Inteiro que define o quantum do escalonador.
*/
vector<processos> RR(vector<processos> &programa, int quantum);

/**
 * @brief Executa um processo.
 * @details Cálcula o tempo executado, aplica o sleep e envia uma mensagem para os processos presentes na fila.
 * @param quantum Quantum do nosso sistema.
 * @param tempo_restante Tempo que o processo ainda tem para ser executado.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição do processo no vetor do programa.
 */
void executa(int quantum, int *tempo_restante, vector<processos> *process, escalonador *escalona, int pos, int *tempo_atual);

#endif
