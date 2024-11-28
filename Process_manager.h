#ifndef _Process_manager_h_
#define _Process_manager_h_

#include <bits/stdc++.h>
using namespace std;

/*  Váriavel global que armazena a quantidade espaço
de memória que será usada para armazenar as mensagens. */
extern const int MEM_LENGTH;

/*  Váriavel que informa qual é o tamanho de cada página */
extern const int PG_LENGTH;

/*  Váriavel que informa qual é a quantidade de recursos disponível*/
extern const int quantidade_de_recursos;

extern const int quantidade_de_nucleos;

/*  Quantidade de memória usada (com operações bitwise para controle de bits) */
extern int mem_usada;

/*  Vetor que simula o espaço na memória RAM */
extern vector<int> Memoria_Principal;

/*  Vetor que simula o espaço na memória secundária */
extern vector<int> Memoria_Secundaria;

/*  Vetor que simula a quantidade disponível de recursos*/
extern vector<int> recursos;

/**
 * @brief Definição da struct da página.
 * @param id Identificador da página.
 * @param presente Booleano que informa se a página está presente
 * na memória RAM ou memória secundária.
 * @param adress Endereço da nossa página.
 * @param ultimo_acesso Variável que informa quando foi o último 
 * momento em que a página foi acessada.
*/
struct pagina
{
    int id; //Id da pagina
    bool presente; // informa se está na memoria RAM 
    int adress;
    int ultimo_acesso;
};

/**
 * @brief Definição da struct endereco
 * @details Esta struct é utilizada como um auxilio para o retorno
 * de algumas funções que precisa retornar um endereço de memória, 
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
 * @brief 
 * @details 
 * @param 
 * @param
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
 * @param id Identificador criado para o processo.
 * @param duracao Tempo de duração do proceso.
 * @param start_time Tempo de início do processo.
 * @param end_time Tempo de finalização do processo.
 * @param estado Estado do processo no processador.
 * @param pipe_menssenger Pipe que guarda o processo remetente das mensagens.
 * @param pipe_message Pipe que guarda as mensagens para o respectivo processo.
 * @param numero_paginas Informa quantas paginas o processo necessita.
 * @param Tabela_paginacao É a tabela de paginação referente ao processo.
 */
struct processos{
    int id;
    int duracao;
    int start_time;
    int end_time;
    int numero_paginas;
    int posicao_tempo_recursos; // Inteiro que armazena 
    string estado;
    queue<int> pipe_messenger;
    queue<string> pipe_message;
    vector<pagina*> Tabela_paginacao;
    vector<int> recursos; // Vetor de recursos do processo
    vector<int> alocado; // Vetor de recursos que guarda quanto de cada recurso está alocado para este processo
    vector<int> precisa; // Vetor de recursos que guarda quanto que cada recurso precisa
    vector<tempo_recurso> tempo_recursos; // Vetor que guarda quando que um recurso será solicitado, e a sua duracao
    vector<int> solicitados; // Vetor que guarda a quantidade de cada recurso q o processo solicita o uso no momento

    processos(int duracao, int inicio) : 
    duracao(duracao), start_time(inicio), estado("Novo") {};
};

/**
 * @brief Função responsável pela criação dos processos.
 * @details Cria os processos.
 * @param duracao Tempo de duração do processo.
 * @param inicio Tempo de início do processo.
 * @param quantidade_paginas Guarda a quantidade de paginas que o processo ocupará
 * @return Retorna um novo processo.
 */
processos cria_process(int duracao, int inicio, int quantidade_paginas);

/**
 * @brief Inicializa o programa.
 * @details Define tempos de duração e início aleatórios para os processos e os organiza.
 * @param quantidade_processos Quantidade de processos do programa.
 * @return Retorna o programa a ser escalonado.
 */
vector<processos> inicia_programa(int quantidade_processos);

/**
 * @brief Cria uma nova página.
 * @return Retorna um ponteiro para uma nova página.
 */
pagina *cria_pg();

/**
 * @brief Adiciona uma página à tabela de paginação de um processo.
 * @param programa Vetor que possui os processos do nosso programa.
 * @param process_id ID do processo.
 * @param page_id ID da página.
 * @param time Tempo atual no programa.
 * @param inicializacao Indica se é na inicialização do processo.
 */
void adiciona_pg(vector<processos> *programa, int process_id, int page_id, int time, bool inicializacao);

/**
 * @brief Insere valor na memória.
 * @param RAM_adress Endereço na memória RAM.
 * @param mem_sec_adress Endereço na memória secundária.
 * @param inicializacao Indica se é na inicialização do processo.
 */
void insere_valor(int RAM_adress, int mem_sec_adress, bool inicializacao);

/**
 * @brief Função de page fault que busca um espaço livre.
 * @param programa Vetor que possui os processos do nosso programa.
 * @param process_id ID do processo.
 * @param page_id ID da página.
 * @param tempo Tempo atual no programa.
 * @param inicializacao Indica se é na inicialização do processo.
 * @return Retorna um endereço de memória.
 */
endereco page_fault(vector<processos> *programa, int process_id, int page_id, int tempo, bool inicializacao);

/**
 * @brief Procura um espaço livre na RAM para uma página
 * @details Caso não haja espaço na memória RAM, ele chama a função
 * page_fault para tentar inserir a página no lugar de uma página antiga.
 * @param programa Vetor que possui os processos do nosso programa.
 * @param process_id ID do processo.
 * @param page_id ID da página.
 * @param tempo Tempo atual no programa.
 * @param inicializacao Indica se é na inicialização do processo.
 * @return Retorna um endereço de memória.
 */
endereco procura_espaco_na_RAM(vector<processos> *programa, int process_id, int page_id, int tempo, bool inicializacao);

/**
 * @brief Acessa a memória de um processo, simulando a leitura.
 * @details Caso a pagina não esteja presente na RAM, ele chama
 * a page_fault para arrumar isso.
 * @param programa Vetor de processos.
 * @param process_id ID do processo.
 * @param tempo Tempo atual no programa.
 */
void acessa_memoria(vector<processos> *programa, int process_id, int tempo);

void define_espaco_recursos(vector<int> *recursos, int max);

void inicializa_recursos_processos(processos *novo);

void define_tempo_recursos(vector<tempo_recurso> *tempos, int recurso, int quant_a_alocar,int duracao_process, int inicio_process);

#endif 
