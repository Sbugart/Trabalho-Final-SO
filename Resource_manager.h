#ifndef _Resource_manager_h_
#define _Resource_manager_h_

#include <bits/stdc++.h>
using namespace std;

struct tempo_recurso;
struct SistemaOperacional;      // Declaração antecipada do SistemaOperacional
struct processos;               // Declaração antecipada de processos

/**
 * @struct resources
 * @brief Definição da struct resources.
 * @details Esta struct armazena as variáveis que um processo necessita para realizar o controle de recursos.
 * A estrutura mantém o controle dos recursos necessários, alocados, solicitados e o tempo de cada recurso solicitado para o processo.
 * 
 * @param posicao_tempo_recursos Inteiro que armazena a posição no vetor de tempo de recursos.
 * @param recursos Vetor que armazena os recursos disponíveis para o processo.
 * @param alocado Vetor que armazena a quantidade de recursos alocados ao processo.
 * @param precisa Vetor que armazena a quantidade de recursos necessários para o processo.
 * @param tempo_recursos Vetor que armazena os tempos de solicitação e duração dos recursos.
 * @param solicitados Vetor que armazena a quantidade de recursos solicitados pelo processo.
 */
struct resources {
    int posicao_tempo_recursos;
    vector<int> recursos; 
    vector<int> alocado;
    vector<int> precisa; 
    vector<tempo_recurso> tempo_recursos; 
    vector<int> solicitados; 
};

/**
 * @brief Define os recursos de um processo, garantindo que não ultrapassem o máximo disponível.
 * @param recursos Ponteiro para o vetor de recursos a ser configurado.
 * @param max Valor máximo de recursos a serem alocados.
 * @param SO Referência para o sistema operacional que contém os recursos disponíveis.
 * @details Esta função preenche o vetor de recursos de um processo com valores aleatórios, garantindo que não ultrapassem os recursos disponíveis no sistema operacional.
 */
void define_espaco_recursos(vector<int> *recursos, int max, SistemaOperacional &SO);

/**
 * @brief Inicializa os recursos de um novo processo.
 * @param novo Ponteiro para o novo processo que está sendo inicializado.
 * @param SO Referência para o sistema operacional que contém os recursos disponíveis.
 * @details A função inicializa os recursos de um novo processo, incluindo a definição de tempos de solicitação e alocação de recursos necessários.
 */
void inicializa_recursos_processos(processos *novo, SistemaOperacional &SO);

/**
 * @brief Define o tempo de solicitação dos recursos para o processo.
 * @param tempos Ponteiro para o vetor que armazenará os tempos de recursos do processo.
 * @param recurso Identificador do recurso.
 * @param quant_a_alocar Quantidade do recurso a ser alocada.
 * @param duracao_process Duração total do processo.
 * @param inicio_process Tempo de início do processo.
 * @details Esta função gera aleatoriamente os tempos de solicitação e a duração para os recursos que o processo irá utilizar.
 */
void define_tempo_recursos(vector<tempo_recurso> *tempos, int recurso, int quant_a_alocar, int duracao_process, int inicio_process);

/**
 * @brief Verifica se o sistema está em um estado seguro.
 * @param SO Referência para o sistema operacional que contém os dados dos processos e recursos.
 * @return Retorna `true` se o sistema estiver em um estado seguro, `false` caso contrário.
 * @details A função verifica se os processos podem ser executados sem risco de deadlock, utilizando o algoritmo de verificação de segurança.
 */
bool safety(SistemaOperacional &SO);

/**
 * @brief Tenta alocar novos recursos para um processo.
 * @param SO Referência para o sistema operacional que contém os recursos disponíveis.
 * @param process Ponteiro para o processo que está solicitando recursos.
 * @return Retorna `true` se a alocação foi bem-sucedida, `false` caso contrário.
 * @details A função tenta alocar os recursos solicitados pelo processo. Se a alocação for segura, os recursos são alocados, caso contrário, a alocação é revertida.
 */
bool novo_recurso(SistemaOperacional &SO, processos *process);

/**
 * @brief Marca as solicitações de recursos feitas por um processo durante sua execução.
 * @details A função marca as solicitações de recursos feitas
 *  por um processo no momento de sua execução.
 * @param SO Referência para o sistema operacional que contém 
 * os dados dos processos e recursos.
 * @param execucao Tempo de execução do processo.
 * @param id Identificador do processo.
 */
void marca_solicitacoes(SistemaOperacional &SO, int execucao, int id);

/**
 * @brief Atualiza os recursos que foram liberados após a execução de um processo.
 * @details A função atualiza a quantidade de recursos liberados após 
 * a execução de um processo e calcula o tempo extra necessário 
 * para o processo ser finalizado.
 * @param SO Referência para o sistema operacional que 
 * contém os dados dos processos e recursos.
 * @param execucao Tempo de execução do processo.
 * @param id Identificador do processo.
 * @param fim_de_processo Flag que indica se o processo foi finalizado.
 * @return Retorna um vetor com a quantidade de recursos liberados.
 */
vector<int> recursos_encerrados(SistemaOperacional &SO, int execucao, int id, bool fim_de_processo);

/**
 * @brief Atualiza o tempo de início definitivo para a alocação de recursos de um processo.
 * @details A função atualiza o tempo definitivo de início da alocação
 *  dos recursos de um processo.
 * @param SO Referência para o sistema operacional que contém os dados
 *  dos processos e recursos.
 * @param pos Posição do processo que está alocando recursos.
 * @param tempo_execucao Tempo de execução do processo.
 */
void atualizaInicioDefinitivo(SistemaOperacional &SO, int pos, int tempo_execucao);

#endif
