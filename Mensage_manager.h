#ifndef _Mensage_manager_h_
#define _Mensage_manager_h_

#include <bits/stdc++.h>
using namespace std;

struct SistemaOperacional;      // Declaração antecipada do SistemaOperacional
struct processos;               // Declaração antecipada de processos

/**
 * @brief Struct para as pipes dos processos
 * @param pipe_messenger Fila de inteiros que guarda quem enviou uma 
 * mensagem para o processo atual
 * @param pipe_message Fila que guarda as mensagens enviadas para
 * o processo atual.
 */
struct pipes{
    queue<int> pipe_messenger;
    queue<string> pipe_message;
};

/**
 * @brief Gerencia comunicação por memória compartilhada entre processos.
 * @details Verifica se há mensagens para o processo atual, processa mensagens recebidas
 *          e envia mensagens para todos os processos na fila do escalonador.
 * @param SO Estrutura do nosso Sistema Operacional
 * @param process_position Índice do processo atual no vetor `programa`.
 */
void mem_comp(SistemaOperacional &SO, int process_position);

/**
 * @brief Processa recebimento de mensagens via memória compartilhada.
 * @details Atualiza o estado da memória compartilhada e gera resposta, se aplicável.
 * @param SO Estrutura do nosso Sistema Operacional
 * @param pos Índice na memória compartilhada.
 * @param process_position Índice do processo no vetor `programa`.
 */
void recebe(SistemaOperacional &SO ,int pos, int process_position);

/**
 * @brief Envia uma mensagem via memória compartilhada.
 * @details Localiza espaço livre na memória e registra a mensagem.
 * @param process Processo que envia a mensagem.
 * @param SO Estrutura do nosso Sistema Operacional
 * @param pos Índice livre na memória compartilhada (ajustado dentro da função).
 * @param p Processo destinatário.
 */
void enviar_mensagem(processos &process, SistemaOperacional &SO, int &pos, processos &p);


/**
 * @brief Gerencia troca de mensagens entre processos via pipe.
 * @details O processo envia mensagens para processos na fila do escalonador.
 * @param programa Vetor contendo os processos do programa.
 * @param escalona Referência ao escalonador.
 * @param pos Índice do processo no vetor `programa`.
 */
void pipe(vector<processos> &programa, escalonador &escalona, int pos);

#endif