#include "Paging.h"
#include "Scheduler.h"
#include "Process_manager.h"
#include "OS.h"

// Função para criar uma nova página
pagina *cria_pg(){
    pagina *nova_pg = new pagina;  // Aloca uma nova página na memória
    return nova_pg;  // Retorna o ponteiro para a página criada
}

// Função para adicionar uma página à tabela de paginação do processo
void adiciona_pg(SistemaOperacional &SO, int process_id ,int page_id, bool inicializacao){
    pagina *aux = cria_pg();  // Cria uma nova página
    // Encontra um espaço na RAM para a página
    endereco adress = procura_espaco_na_RAM(SO, process_id, page_id, SO.tempo_atual, inicializacao);
    aux->adress = adress.adress;  // Define o endereço da página
    aux->id = page_id;  // Define o ID da página
    aux->presente = adress.pertence == 1 ? true : false;  // Define se a página está presente na RAM
    aux->ultimo_acesso = SO.tempo_atual;  // Define o tempo do último acesso
    // Adiciona a página à tabela de paginação do processo
    SO.programa[process_id].Tabela_paginacao.push_back(aux);
    cout << "Pagina " << page_id + 1 << " do processo " << process_id + 1 << " criada com sucesso!" << endl;
}

// Função para inserir um valor na memória, seja inicializando ou restaurando dados
void insere_valor(SistemaOperacional &SO, int RAM_adress, int mem_sec_adress, bool inicializacao){
    vector<int> aux(SO.memoria.PG_LENGTH);  // Cria um vetor para armazenar dados temporários
    for(int pos = 0; pos < (int)SO.memoria.PG_LENGTH; pos++){
        aux[pos] = SO.memoria.Memoria_Principal[RAM_adress + pos];  // Armazena os dados da RAM temporariamente
        if(inicializacao){
            srand(time(0));  // Gera um número aleatório
            SO.memoria.Memoria_Principal[RAM_adress + pos] = rand();  // Inicializa os dados na RAM com valores aleatórios
        }
        else{
            SO.memoria.Memoria_Principal[RAM_adress + pos] = SO.memoria.Memoria_Secundaria[mem_sec_adress + pos];  // Restaura dados da memória secundária
        }
    }
}

// Função para lidar com page faults (falta de página na RAM)
endereco page_fault(SistemaOperacional &SO, int process_id, int page_id, bool inicializacao){
    int flag = 0;  // Flag para verificar se foi encontrado um espaço
    endereco pos;
    pos.pertence = 0;  // Inicializa a variável de endereço
    cout << "Tentando achar um espaco livre" << endl;
    // Verifica outros processos na memória para liberar páginas
    for(int i = 0; i < (int)SO.programa.size(); i++){
        if(process_id != i){  // Verifica se não é o próprio processo
            for(int j = 0; j < (int)SO.programa[i].numero_paginas; j++){
                if(SO.programa[i].Tabela_paginacao[j]->ultimo_acesso < SO.tempo_atual){
                    pos.adress = SO.programa[i].Tabela_paginacao[j]->adress;  // Seleciona o endereço da página
                    pos.pertence = 1;  // Define que a página pertence à RAM
                    flag = 1;
                    // Insere os valores da página do processo em questão na RAM
                    insere_valor(SO, pos.adress,SO.programa[process_id].Tabela_paginacao[page_id]->adress, inicializacao);
                    return pos;  // Retorna o endereço encontrado
                }
            }
        }
    }
    cout << "Nao foi encontrado um local na RAM que tenha vindo antes de " << SO.tempo_atual << endl;
    if(!flag){
        if(inicializacao){  // Se não encontrar um espaço, inicializa uma nova página
            pos.adress = SO.memoria.Memoria_Secundaria.size();
            for(int i = 0; i < SO.memoria.PG_LENGTH; i++){
                srand(time(0));  // Preenche a memória secundária com dados aleatórios
                SO.memoria.Memoria_Secundaria.push_back(rand());
            }
        }
    }
    return pos;  // Retorna o endereço
}

// Função para procurar espaço na memória RAM para uma página
endereco procura_espaco_na_RAM(SistemaOperacional &SO, int process_id, int page_id, int tempo, bool inicializacao){
    int toda_mem_usada = 63;  // Representação de memória usada (um total de 6 páginas)
    endereco espaco;
    cout << "Procurando espaco na memoria RAM" << endl;
    // Verifica se há memória livre na RAM
    if(toda_mem_usada != SO.memoria.mem_usada){
        for(int i = 0; i < 6; i++){
            if(!(SO.memoria.mem_usada & (1 << i))){  // Verifica se o espaço está disponível
                cout << "A memoria RAM ainda possui um espaco vazio" << endl;
                SO.memoria.mem_usada |= (1 << i);  // Marca o espaço como utilizado
                espaco.adress = i * SO.memoria.PG_LENGTH;  // Calcula o endereço do espaço livre
                espaco.pertence = 1;  // Marca o espaço como pertencente à RAM
                return espaco;  // Retorna o endereço encontrado
            }
        }
    }
    cout << "Memoria RAM ja esta cheia..." << endl;
    espaco = page_fault(SO, process_id, page_id, inicializacao);  // Chama a função de page fault se a memória estiver cheia
    cout << "Foi retornado um endereco de memoria" << endl;
    return espaco;  // Retorna o endereço encontrado
}

// Função para acessar páginas da memória para leitura
void acessa_memoria(SistemaOperacional &SO, int process_id){
    int quant_pgs = SO.programa[process_id].Tabela_paginacao.size();  // Número de páginas no processo
    
    int pgs_acessadas = rand() % quant_pgs + 1;  // Determina o número de páginas a serem acessadas aleatoriamente
    for(int i = 0; i < pgs_acessadas; i++){
        if(SO.programa[process_id].Tabela_paginacao[i]->presente){  // Verifica se a página está na RAM
            cout << "Lendo o item na RAM" << endl;
            for(int j = 0; j < SO.memoria.PG_LENGTH; j++){
                // Simula a leitura de dados na RAM
                int leitura = SO.memoria.Memoria_Principal[SO.programa[process_id].Tabela_paginacao[i]->adress + j];
            }
        }
        else{  // Se a página não estiver na RAM, faz um page fault
            cout << "A memória RAM nao contem a pagina desejada..." << endl;
            endereco espaco = page_fault(SO, process_id, i, false);  // Busca a página na memória secundária
            SO.programa[process_id].Tabela_paginacao[i]->adress = espaco.adress;  // Atualiza o endereço da página
            SO.programa[process_id].Tabela_paginacao[i]->presente = espaco.pertence;  // Marca a página como presente ou não
        }
    }
}
