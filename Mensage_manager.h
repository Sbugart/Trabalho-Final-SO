#ifndef _Mensage_manager_h_
#define _Mensage_manager_h_

#include "Scheduler.h"

/**
 * @brief Envia e recebe mensagem por Memória compartilhada
 * @details Olha se possui uma mensagem para ler, se for verdade ele chama a função recebe.
 *          Além disso, ele olha os processos presentes na fila, e envia uma mensagem para eles.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos_pro Posição do processo no vetor do programa.
 */
void mem_comp(vector<processos> &programa, escalonador &escalona, int process_position);

/**
 * @brief Recebe mensagem
 * @details Envia uma mensagem de confirmação de mensagem por memoria compartilhada
 *          caso tenha recebido uma solitação de um processo que ainda não foi finalizado.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição no vetor da memória compartilhada.
 * @param pos_pro Posição do processo no vetor do programa.
 */
void recebe(vector<processos> &process, escalonador &escalona,int pos, int pos_pro);

/**
 * @brief Envia uma mensagem pelo método de memória compartilhada.
 * @details Envia uma mensagem para todos os processos que estão na fila de processos
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição no vetor de memória compartilhada
 * @param p Processo que vai receber a mensagem
 */
void enviar_mensagem(processos &process, escalonador &escalona, int &pos, processos &p);


/**
 * @brief Troca de mensagens dos processos pela pipe.
 * @details O processo mensageiro envia uma mensagem para os processos da fila.
 * @param process Vetor que possui os processos do nosso programa.
 * @param escalona Ponteiro para os dados do escalonador.
 * @param pos Posição do processo no vetor do programa.
 */
void pipe(vector<processos> &process, escalonador &escalona, int pos);

#endif