#ifndef _escalonamento_h_
#define _escalonamento_h_

#include "processos.h"
#include <windows.h>

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
void RR(vector<processos> &programa, int quantum);

/**
 * @brief Executa um processo.
 * @details Cálcula o tempo executado, aplica o sleep e envia uma mensagem para os processos presentes na fila.
 * @param quantum Quantum do nosso sistema.
 * @param tempo_restante Tempo que o processo ainda tem para ser executado.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição do processo no vetor do programa.
 * @return Retorna o tempo executado.
 */
int executa(int quantum, int *tempo_restante, vector<processos> *process, escalonador *escalona, int pos);

/**
 * @brief Envia e recebe mensagem por Memória compartilhada
 * @details Olha se possui uma mensagem para ler, se for verdade ele chama a função recebe.
 *          Além disso, ele olha os processos presentes na fila, e envia uma mensagem para eles.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos_pro Posição do processo no vetor do programa.
 */
void mem_comp(vector<processos> *process, escalonador *escalona, int pos_pro);

/**
 * @brief Recebe mensagem
 * @details Envia uma mensagem de confirmação de mensagem por memoria compartilhada
 *          caso tenha recebido uma solitação de um processo que ainda não foi finalizado.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição no vetor da memória compartilhada.
 * @param pos_pro Posição do processo no vetor do programa.
 */
void recebe(vector<processos> *process, escalonador *escalona,int pos, int pos_pro);

/**
 * @brief Envia uma mensagem
 * @details Envia uma mensagem para todos os processos que estão na fila de processos
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição no vetor de memória compartilhada
 * @param p Processo que vai receber a mensagem
 */
void enviar_mensagem(processos *process, escalonador *escalona, int *pos, processos* p);

/**
 * @brief Troca de mensagens dos processos pela pipe.
 * @details O processo mensageiro envia uma mensagem para os processos da fila.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição do processo no vetor do programa.
 */
void pipe(vector<processos> *process, escalonador *escalona, int pos);
#endif
