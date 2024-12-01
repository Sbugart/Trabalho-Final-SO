#ifndef _Scheduler_h_
#define _Scheduler_h_

#include <bits/stdc++.h>
using namespace std;

// Declarações antecipadas de estruturas que serão utilizadas.
struct SistemaOperacional;      // Declaração antecipada da struct SistemaOperacional
struct memorymanager;           // Declaração antecipada da struct memorymanager
struct processos;               // Declaração antecipada da struct processos

/**
 * @brief Definição da struct escalonador.
 * Esta struct gerencia a fila de processos a serem executados, além de armazenar
 * informações como o tempo de espera (wt), o tempo total de execução (tat),
 * e o tempo restante (tempo_restante) para cada processo.
 * @param fila Fila de processos que serão executados.
 * @param memoria_mensagens Espaço de memória fixo para armazenar mensagens dos processos.
 * @param usado Vetor indicando se uma posição na memória de mensagens está sendo usada.
 * @param receber Vetor que armazena o ID do processo que recebeu uma informação.
 * @param wt Vetor que guarda o tempo de espera de cada processo.
 * @param tat Vetor que guarda o tempo total de execução de cada processo.
 * @param tempo_restante Vetor que guarda o tempo restante para cada processo ser concluído.
 * @attention Na função de inicialização, além de inicializar os vetores, o tempo_restante já é configurado com os valores de duração dos processos.
 */
struct escalonador
{
    queue<processos*> fila;                  
    vector<string> memoria_mensagens;         
    vector<int> usado, receber;            
    vector<int> wt, tat, tempo_restante;        

    // Construtor para inicialização dos vetores
    escalonador(int tamanho, vector<processos> &programa, const int MEM_LENGTH)
     : memoria_mensagens(MEM_LENGTH), usado(MEM_LENGTH, 0),
       receber(MEM_LENGTH), wt(tamanho, 0), tat(tamanho, 0), 
       tempo_restante(tamanho, 0) {
        // Inicializa o vetor tempo_restante com a duração de cada processo
        transform(programa.begin(), programa.end(), tempo_restante.begin(),
        [](const processos &p){return p.duracao;});
    };
};

/**
 * @brief Aplica o algoritmo de escalonamento Round Robin (RR).
 * Esta função simula o escalonamento de processos utilizando o algoritmo Round Robin,
 * onde os processos são executados por um período de tempo (quantum) em rodízio.
 * @param SO Estrutura do Sistema Operacional que gerencia os processos e o escalonador.
 */
void RR(SistemaOperacional &SO);

/**
 * @brief Executa um processo.
 * Esta função simula a execução de um processo, determinando quanto tempo ele será executado
 * de acordo com o quantum, e realizando as operações necessárias, como a alocação de recursos,
 * a marcação de solicitações e o gerenciamento da memória.
 * @param SO Estrutura do Sistema Operacional que gerencia os processos.
 * @param pos Índice do processo a ser executado.
 */
void executa(SistemaOperacional &SO, int pos);

/**
 * @brief Adiciona um processo na fila do escalonador.
 * Esta função adiciona os processos que já deveriam estar executando na fila de execução,
 * verificando seu tempo de início e se já podem ser escalonados.
 * @param SO Estrutura do Sistema Operacional.
 * @param pos Índice do próximo processo a ser verificado.
 * @param flag Indicador de controle para a adição do processo.
 */
void adiciona_process_na_fila(SistemaOperacional &SO, int &pos, int flag);

#endif
