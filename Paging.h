#ifndef _Paging_h_
#define _Paging_h_

#include <bits/stdc++.h>
using namespace std;

struct SistemaOperacional;      ///< Declaração antecipada do SistemaOperacional
struct memorymanager;           ///< Declaração antecipada de memorymanager
struct processos;               ///< Declaração antecipada de processos
struct endereco;                ///< Declaração antecipada de endereco

/**
 * @brief Estrutura que gerencia a memória do sistema.
 * 
 * @param mem_usada Armazena as posições da memória principal que foram utilizadas.
 * @param PG_LENGTH Tamanho de uma página de memória.
 * @param RAM_SIZE Tamanho da memória RAM (total de páginas).
 * @param Memoria_Principal Representa a memória RAM do sistema.
 * @param Memoria_Secundaria Representa a memória secundária (disco ou similar).
 */
struct memorymanager
{
    int mem_usada; 

    static const int PG_LENGTH = 4000; 
    static const int RAM_SIZE = 6 * PG_LENGTH; 

    vector<int> Memoria_Principal; 
    vector<int> Memoria_Secundaria; 

    /// Construtor da estrutura memorymanager.
    memorymanager() : mem_usada(0), Memoria_Principal(RAM_SIZE, -1) {};
};

/**
 * @brief Definição da estrutura de uma página de memória.
 * 
 * @param id Identificador único da página.
 * @param presente Indica se a página está presente na memória RAM (true) ou na memória secundária (false).
 * @param adress Endereço de memória da página.
 * @param ultimo_acesso Registra o último momento em que a página foi acessada.
 */
struct pagina
{
    int id;               ///< Identificador único da página.
    bool presente;        ///< Indica se a página está na memória RAM (true) ou na memória secundária (false).
    int adress;           ///< Endereço de memória da página.
    int ultimo_acesso;    ///< Tempo do último acesso à página.
};

/**
 * @brief Cria uma nova página de memória.
 * 
 * @return Retorna um ponteiro para a nova página criada.
 */
pagina *cria_pg();

/**
 * @brief Adiciona uma página à tabela de paginação de um processo.
 * 
 * @param SO Estrutura que representa o Sistema Operacional.
 * @param process_id Identificador do processo que está associando a página.
 * @param page_id Identificador da página a ser adicionada.
 * @param inicializacao Indica se a operação ocorre durante a inicialização do processo.
 */
void adiciona_pg(SistemaOperacional &SO, int process_id, int page_id, bool inicializacao);

/**
 * @brief Insere valores na memória, seja na RAM ou na memória secundária.
 * 
 * @param SO Estrutura que representa o Sistema Operacional.
 * @param RAM_adress Endereço na memória RAM onde o valor será armazenado.
 * @param mem_sec_adress Endereço na memória secundária de onde o valor será copiado.
 * @param inicializacao Indica se a operação ocorre durante a inicialização do processo.
 */
void insere_valor(SistemaOperacional &SO, int RAM_adress, int mem_sec_adress, bool inicializacao);

/**
 * @brief Função que trata o "page fault" ao buscar um espaço livre para uma página.
 * 
 * @details Caso não haja espaço suficiente na memória RAM, a função tenta liberar espaço removendo uma página antiga.
 * 
 * @param SO Estrutura que representa o Sistema Operacional.
 * @param process_id Identificador do processo que causou o page fault.
 * @param page_id Identificador da página que precisa ser carregada.
 * @param inicializacao Indica se o page fault ocorreu durante a inicialização do processo.
 * @return Retorna o endereço de memória onde a página será carregada.
 */
endereco page_fault(SistemaOperacional &SO, int process_id, int page_id, bool inicializacao);

/**
 * @brief Procura por um espaço livre na memória RAM para alocar uma página.
 * 
 * @details Caso não haja espaço na memória RAM, a função chama a função page_fault para tentar inserir a página em um espaço livre.
 * 
 * @param SO Estrutura que representa o Sistema Operacional.
 * @param process_id Identificador do processo que está requisitando espaço.
 * @param page_id Identificador da página que precisa ser alocada.
 * @param tempo Tempo atual no sistema.
 * @param inicializacao Indica se a operação ocorre durante a inicialização do processo.
 * @return Retorna o endereço de memória onde a página será alocada.
 */
endereco procura_espaco_na_RAM(SistemaOperacional &SO, int process_id, int page_id, int tempo, bool inicializacao);

/**
 * @brief Acessa a memória de um processo, simulando a leitura de uma página.
 * 
 * @details Caso a página não esteja presente na RAM, a função chama a page_fault para corrigir isso.
 * 
 * @param SO Estrutura com as informações do Sistema Operacional.
 * @param process_id Identificador do processo que está tentando acessar a memória.
 */
void acessa_memoria(SistemaOperacional &SO, int process_id);

#endif
