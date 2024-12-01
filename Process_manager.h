#ifndef _Process_manager_h_
#define _Process_manager_h_

#include <bits/stdc++.h>
#include <random>
#include "Mensage_manager.h"

struct pagina;
struct resources;
struct SistemaOperacional;      

using namespace std;

/**
 * @brief Definição da struct endereco
 * @details Esta struct é utilizada como um auxílio para o retorno
 * de algumas funções que precisam retornar um endereço de memória,
 * diferenciando se ele está presente na RAM ou na memória secundária.
 * @param adress Endereço da memória, independente do tipo de memória.
 * @param pertence Variável utilizada para informar se o adress pertence
 * a memória RAM ou memória secundária.
 */
struct endereco
{
    int adress; 
    int pertence; 
};

/**
 * @brief Definição da struct tempo_recurso
 * @details Esta struct é utilizada para armazenar as informações relacionadas
 * ao tempo de uso de um recurso por um processo.
 * @param recurso Identificador do recurso utilizado.
 * @param inicio_esperado Tempo esperado para o início do uso do recurso.
 * @param duracao Duração prevista para o uso do recurso.
 * @param inicio_definitivo Tempo real em que o recurso foi iniciado.
 */
struct tempo_recurso
{
    int recurso; 
    int inicio_esperado; 
    int duracao; 
    int inicio_definitivo; 
};

/**
 * @brief Definição da struct processos
 * @details Esta struct contém todas as informações sobre os processos
 * que estão sendo gerenciados pelo sistema operacional.
 * @param id Identificador do processo.
 * @param duracao Tempo de duração do processo.
 * @param start_time Tempo de início do processo.
 * @param end_time Tempo de finalização do processo.
 * @param estado Estado atual do processo.
 * @param recurso Estrutura de recursos relacionados ao processo.
 * @param process_pipes Estrutura de pipes para comunicação entre processos.
 * @param numero_paginas Número de páginas que o processo ocupa na memória.
 * @param Tabela_paginacao Tabela de paginação associada ao processo.
 */
struct processos
{
    int id; 
    int duracao; 
    int start_time; 
    int end_time; 
    int numero_paginas; 
    string estado; 
    resources recurso; 
    pipes process_pipes; 
    vector<pagina*> Tabela_paginacao; 

    processos(int duracao, int inicio) : 
    duracao(duracao), start_time(inicio), estado("Novo") {};
};

/**
 * @brief Função responsável pela criação de processos.
 * @details Esta função cria um novo processo com os parâmetros fornecidos,
 * inicializando os recursos necessários e configurando suas propriedades.
 * @param duracao Tempo de duração do processo.
 * @param inicio Tempo de início do processo.
 * @param quantidade_paginas Número de páginas que o processo ocupará.
 * @param SO Sistema operacional que gerencia o processo.
 * @return Retorna o novo processo criado.
 */
processos cria_process(int duracao, int inicio, int quantidade_paginas, SistemaOperacional &SO);

/**
 * @brief Função que inicializa o programa com uma quantidade de processos.
 * @details Cria uma lista de processos com duração e tempo de início aleatórios, 
 * organiza-os pela ordem de início e inicializa suas tabelas de paginação.
 * @param SO Sistema operacional que gerencia os processos.
 * @return Retorna um vetor com os processos do programa.
 */
vector<processos> inicia_programa(SistemaOperacional &SO);

/**
 * @brief Função que finaliza o processo no sistema operacional.
 * @details Esta função marca o processo como finalizado, libera os recursos utilizados
 * e remove as páginas de sua tabela de paginação.
 * @param SO Sistema operacional que gerencia os processos.
 * @param flag Flag que indica o estado do processo (0 para finalizado).
 * @param pos Posição do processo a ser finalizado no vetor de processos.
 */
void finaliza_process(SistemaOperacional &SO, int &flag, int pos);

/**
 * @brief Função que suspende o processo no sistema operacional.
 * @details A função suspende o processo, coloca-o na fila de suspensão e altera seu estado.
 * @param SO Sistema operacional que gerencia os processos.
 * @param pos Posição do processo a ser suspenso no vetor de processos.
 */
void suspende_process(SistemaOperacional &SO, int pos);

/**
 * @brief Função de impressão de resultados.
 * @details Imprime uma mensagem com um identificador seguido de um vetor de inteiros.
 * @param texto Mensagem a ser impressa antes do vetor de inteiros.
 * @param resultado Vetor de inteiros a ser impresso.
 * @param id Identificador do processo ou da operação que está sendo exibida.
 */
void print(string texto, vector<int> resultado, int id);

#endif
