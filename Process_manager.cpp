#include "Process_manager.h"  // Inclusão do cabeçalho que define a estrutura e funções relacionadas aos processos
#include "Resource_manager.h" // Inclusão do cabeçalho que define a estrutura e funções relacionadas aos recursos
#include "OS.h"               // Inclusão do cabeçalho que define o sistema operacional
#include "Scheduler.h"        // Inclusão do cabeçalho que define o escalonador
#include "Paging.h"           // Inclusão do cabeçalho que define a paginação e gerenciamento de memória

// Função para criar um novo processo com duração, tempo de início e número de páginas especificados
processos cria_process(int duracao, int inicio, int quantidade_paginas, SistemaOperacional &SO) {
    processos novo(duracao, inicio);  // Criação de um novo processo com a duração e o tempo de início fornecidos
    novo.numero_paginas = quantidade_paginas;  // Definição do número de páginas para o processo
    cout << "Um novo processo foi criado com " << quantidade_paginas << " paginas" << endl;
    
    inicializa_recursos_processos(&novo, SO);  // Inicializa os recursos necessários para o processo
    return novo;  // Retorna o novo processo criado
}

// Função para inicializar o programa (vetor de processos)
vector<processos> inicia_programa(SistemaOperacional &SO) {
    vector<processos> programa;  // Vetor para armazenar os processos a serem criados
    int duracao, inicio, quant_pgs;  // Variáveis auxiliares para armazenar os atributos de cada processo

    // Criação dos processos com duração, tempo de início e número de páginas aleatórios
    for(int i = 0; i < SO.quantidade_de_processos; i++) {
        duracao = 1 + rand() % 25;  // Duração aleatória do processo (de 1 a 25)
        inicio = rand() % 20;       // Tempo de início aleatório (de 0 a 19)
        quant_pgs = (rand() % 2 + 1); // Número de páginas aleatório (1 ou 2)
        programa.push_back(cria_process(duracao, inicio, quant_pgs, SO));  // Criação e adição do processo ao vetor
    }

    // Ordenação dos processos com base no tempo de início
    sort(programa.begin(), programa.end(), [] (processos a, processos b) {
        return a.start_time < b.start_time;  // Ordena em ordem crescente de start_time
    });

    // Atribui IDs aos processos e inicializa as páginas para cada processo
    for(int i = 0; i < SO.quantidade_de_processos; i++) {
        programa[i].id = i + 1;  // Atribui o ID (começando de 1)
        for(int j = 0; j < programa[i].numero_paginas; j++) {
            // Adiciona páginas aos processos (tempo de paginação inicial é 0 para todos)
            adiciona_pg(SO, i, j, true); 
        }
    }

    return programa;  // Retorna o vetor de processos inicializados
}

// Função para finalizar um processo
void finaliza_process(SistemaOperacional &SO, int &flag, int pos) {
    // Marca o tempo final do processo
    SO.programa[pos].end_time = SO.tempo_atual;
    
    // Calcula o Turnaround Time (tat) e o Waiting Time (wt) do processo
    SO.escalona.tat[pos] = SO.tempo_atual - SO.programa[pos].start_time;
    SO.escalona.wt[pos] = SO.escalona.tat[pos] - SO.programa[pos].duracao;

    // Remove as páginas alocadas para o processo da tabela de paginação
    for(int i = 0; i < SO.programa[pos].numero_paginas; i++) {
        if(SO.programa[pos].Tabela_paginacao.back()->presente) {
            // Imprime a página removida e ajusta o uso da memória
            cout << "Removendo pagina " << SO.programa[pos].Tabela_paginacao.back()->id
            << " do adress: " << SO.programa[pos].Tabela_paginacao.back()->adress / SO.memoria.PG_LENGTH << endl;
            
            // Marca a página como não utilizada na memória
            SO.memoria.mem_usada ^= (1 << SO.programa[pos].Tabela_paginacao.back()->adress / SO.memoria.PG_LENGTH); 
        }
        SO.programa[pos].Tabela_paginacao.pop_back();  // Remove a página da tabela
    }

    // Adiciona o processo à ordem de saída e marca como finalizado
    SO.ordem_de_saida.push_back(SO.programa[pos]);
    SO.programa[pos].estado = "Finalizado";
    
    // Define a flag como 0 para indicar que o processo foi finalizado
    flag = 0;

    // Libera os recursos que estavam sendo utilizados pelo processo
    vector<int> liberado = recursos_encerrados(SO, 0, pos, true);
    cout << "Quantidade por recurso liberada do Processo " << (SO.programa)[pos].id << ": ";
    for(int i = 0; i < SO.quantidade_de_recursos; i++) {
        cout << " " << liberado[i];  // Imprime a quantidade de recursos liberados
    }
    cout << endl;
}

// Função para suspender um processo
void suspende_process(SistemaOperacional &SO, int pos) {
    // Coloca o processo na fila do escalonador
    SO.escalona.fila.push(&SO.programa[pos]);
    // Marca o processo como suspenso
    SO.programa[pos].estado = "Suspenso";
    
    // Adiciona o processo à ordem de saída, mesmo que não tenha finalizado
    SO.ordem_de_saida.push_back(SO.programa[pos]);
}

// Função auxiliar para imprimir resultados em formato específico
void print(string texto, vector<int> resultado, int id) {
    cout << texto << id << ":";  // Imprime o texto junto com o ID
    for(const auto& valor : resultado) 
        cout << " " << valor;  // Imprime os valores do vetor de resultados
    cout << endl;  // Finaliza a impressão com uma nova linha
}
