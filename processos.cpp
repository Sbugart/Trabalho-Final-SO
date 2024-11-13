#include "escalonamento.h"

processos cria_process(int duracao, int inicio, int quantidade_paginas){
    processos novo(duracao, inicio);
    novo.numero_paginas = quantidade_paginas;
    cout << "Um novo processo foi criado com " << quantidade_paginas << " paginas" << endl;
    return novo;
}

vector<processos> inicia_programa(int quantidade_processos){
    vector<processos> programa;
    int duracao, inicio, quant_pgs;
    srand(time(0));

    for(int i = 0; i < quantidade_processos; i++){
        duracao = 1 + rand() % 25;
        inicio = rand() % 20;
        quant_pgs = (rand() % 2 + 1);
        programa.push_back(cria_process(duracao, inicio, quant_pgs));
    }

    sort(programa.begin(), programa.end(), [] (processos a, processos b){
        return a.start_time < b.start_time;
    });

    for(int i = 0; i < quantidade_processos; i++){
        programa[i].id = i + 1;
        for(int j = 0; j < programa[i].numero_paginas; j++){
            adiciona_pg(&programa, i, j, 0, true); //Como todos os processos ficam como novo juntos,
                                        // Todos possuem o tempo de paginação como 0.
        }
    }

    return programa;
}

pagina *cria_pg(){
    pagina *nova_pg = new pagina;
    return nova_pg;
}

void adiciona_pg(vector<processos> *programa, int process_id ,int page_id, int time, bool inicializacao){
    pagina *aux = cria_pg();
    endereco adress = procura_espaco_na_RAM(programa, process_id, page_id,time, inicializacao);
    aux->adress = adress.adress;
    aux->id = page_id;
    aux->presente = adress.pertence == 1 ? true : false;
    aux->ultimo_acesso = time;
    (*programa)[process_id].Tabela_paginacao.push_back(aux);
    cout << "Pagina " << page_id + 1 << " do processo " << process_id + 1 << " criada com sucesso!" << endl;
}

void insere_valor(int RAM_adress, int mem_sec_adress, bool inicializacao){
    vector<int> aux(PG_LENGTH);
    for(int pos = 0; pos < (int)PG_LENGTH; pos++){
        aux[pos] = Memoria_Principal[RAM_adress + pos];
        if(inicializacao){
            srand(time(0));
            Memoria_Principal[RAM_adress + pos] = rand();
        }
        else{
            Memoria_Principal[RAM_adress + pos] = Memoria_Secundaria[mem_sec_adress + pos];
        }
    }
}

endereco page_fault(vector<processos> *programa, int process_id, int page_id,int tempo, bool inicializacao){
    int flag = 0;
    endereco pos;
    pos.pertence = 0;
    cout << "Tentando achar um espaco livre" << endl;
    for(int i = 0; i < (int)(*programa).size(); i++){
        if(process_id != i){
            for(int j = 0; j < (int)(*programa)[i].numero_paginas; j++){
                if((*programa)[i].Tabela_paginacao[j]->ultimo_acesso < tempo){
                    pos.adress = (*programa)[i].Tabela_paginacao[j]->adress;
                    pos.pertence = 1;
                    flag = 1;
                    insere_valor(pos.adress,(*programa)[process_id].Tabela_paginacao[page_id]->adress, inicializacao);
                    return pos;
                }
            }
        }
    }
    cout << "Nao foi encontrado um local na RAM que tenha vindo antes de " << tempo << endl;
    if(!flag){
        if(inicializacao){
            pos.adress = Memoria_Secundaria.size();
            for(int i = 0; i < PG_LENGTH; i++){
                srand(time(0));
                Memoria_Secundaria.push_back(rand());
            }
        }
    }
    return pos;
}

endereco procura_espaco_na_RAM(vector<processos> *programa, int process_id, int page_id,int tempo, bool inicializacao){
    int toda_mem_usada = 63; // 1 + 2 + 4 + 8 + 16 + 32, que seria o valor 000..00111111
    endereco espaco;
    cout << "Procurando espaco na memoria RAM" << endl;
    if(toda_mem_usada != mem_usada){
        for(int i = 0; i < 6; i++){
            if(!(mem_usada & (1 << i))){
                cout << "A memoria RAM ainda possui um espaco vazio" << endl;
                mem_usada |= (1 << i);
                espaco.adress = i * PG_LENGTH;
                espaco.pertence = 1; //Espaço na RAM
                return espaco;
            }
        }
    }
    cout << "Memoria RAM ja esta cheia..." << endl;
    espaco = page_fault(programa, process_id, page_id, tempo, inicializacao);
    cout << "Foi retornado um endereco de memoria" << endl;
    return espaco;
}

void acessa_memoria(vector<processos> *programa, int process_id, int tempo){
    int quant_pgs = (*programa)[process_id].Tabela_paginacao.size();
    srand(time(0));
    int pgs_acessadas = rand() % quant_pgs + 1; //Quantidade de paginas que o processo vai acessar
    for(int i = 0; i < pgs_acessadas; i++){
        if((*programa)[process_id].Tabela_paginacao[i]->presente){
            cout << "Lendo o item na RAM" << endl;
            for(int j = 0; j < PG_LENGTH; j++){
                // Simula a leitura das informações salvas na RAM
                int leitura = Memoria_Principal[(*programa)[process_id].Tabela_paginacao[i]->adress + j];
            }
        }
        else{
            cout << "A memória RAM nao contem a pagina desejada..." << endl;
            endereco espaco = page_fault(programa, process_id, i, tempo, false);
            (*programa)[process_id].Tabela_paginacao[i]->adress = espaco.adress;
            (*programa)[process_id].Tabela_paginacao[i]->presente = espaco.pertence;
        }
    }
}