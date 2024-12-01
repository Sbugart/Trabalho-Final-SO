#ifndef _OS_h_
#define _OS_h_

// Inclusões de bibliotecas padrão e outras dependências necessárias
#include <bits/stdc++.h>
using namespace std;
#include "Paging.h"  // Inclui o cabeçalho que define a estrutura memorymanager

// Declarações antecipadas das estruturas que serão usadas no código
struct escalonador;    // Declaração antecipada da estrutura escalonador
struct processos;      // Declaração antecipada da estrutura processos


// Esta estrutura contém os parâmetros e objetos necessários para simular um sistema operacional,
// incluindo a memória, recursos, processos, e o escalonador que gerencia os processos.
/**
 * @brief Estrutura que representa o Sistema Operacional
 * @details Esta estrutura contém os parâmetros e objetos necessários para simular um sistema operacional,
 * incluindo a memória, recursos, processos, e o escalonador que gerencia os processos.
 * @param quantidade_de_processos Número total de processos que serão gerenciados.
 * @param quantum Tempo de execução máximo para cada processo (em unidades de tempo)
 * @param max_quant_por_recursos Quantidade máxima de recursos por tipo.
 * @param quantidade_de_recursos Número total de tipos de recursos no sistema
 * @param quantidade_de_nucleos Número de núcleos de processamento disponíveis no sistema
 * @param MEM_LENGTH Tamanho total da memória disponível no sistema
 * @param tempo_atual Tempo atual do sistema, utilizado para gerenciar a execução
 * @param memoria Estrutura que gerencia a memória do sistema
 * @param escalona Estrutura responsável pelo escalonamento dos processos.
 * @param recursos Vetor que mantém o estado dos recursos no sistema
 * @param programa Vetor de processos a serem executados no sistema
 * @param ordem_de_saida Vetor que mantém a ordem de finalização dos processos
*/
struct SistemaOperacional
{
    // Atributos do Sistema Operacional
    const int quantidade_de_processos;      
    const int quantum;               
    const int max_quant_por_recursos; 
    const int quantidade_de_recursos;     
    const int quantidade_de_nucleos;        
    const int MEM_LENGTH;                   
    int tempo_atual;                        
    memorymanager memoria;               
    escalonador escalona;             
    vector<int> recursos;            
    vector<processos> programa;         
    vector<processos> ordem_de_saida;       

    // Construtor da estrutura SistemaOperacional
    // O parâmetro escalonador é inicializado com a quantidade de processos e o programa a ser executado.
    SistemaOperacional(const int quant_processos, const int quantum,
                        const int max_recursos, const int quant_recursos, 
                        const int tamanho_mensagens, const int nucleos)
        : quantidade_de_processos(quant_processos), quantum(quantum), 
          max_quant_por_recursos(max_recursos), 
          quantidade_de_recursos(quant_recursos),
          quantidade_de_nucleos(nucleos), MEM_LENGTH(tamanho_mensagens),
          tempo_atual(0), escalona(quant_processos, programa, tamanho_mensagens) {}
};

// Fim do arquivo de cabeçalho
#endif
