#include "Scheduler.h"
#include "Resource_manager.h"
#include "Mensage_manager.h"

void executa(int quantum, int *tempo_restante, vector<processos> *process, escalonador *escalona, int pos, int *tempo_atual){
    int execucao = min(quantum, *tempo_restante);
    
    vector<int> liberado = recursos_encerrados(process, execucao, pos, false, &(*escalona).tempo_restante[pos]);
    cout << "Quantidade por recurso liberada do Processo " << (*process)[pos].id << ": ";
    for(int i = 0; i < quantidade_de_recursos; i++){
        cout << " " << liberado[i];
    }
    cout << endl;

    marca_solicitacoes(process, (*tempo_atual), execucao, pos);
    cout << "Vetor de solicitados do Processo " << (*process)[pos].id << ":";
    for(int i = 0; i < quantidade_de_recursos; i++){
        cout << " " << (*process)[pos].solicitados[i];
    }
    cout << endl;

    if (novo_recurso(process, *tempo_atual, &((*process)[pos]))){
        cout << "Processo " << (*process)[pos].id << " alocando recurso " << endl;
        for(auto& recursos : (*process)[pos].tempo_recursos){
            if(recursos.inicio_definitivo == -2){
                if(recursos.inicio_esperado <= (*tempo_atual)) recursos.inicio_definitivo = execucao;
                else recursos.inicio_definitivo = (*tempo_atual) + execucao - recursos.inicio_esperado;
            }
        }
        cout << "Recursos restantes:";
        for(int i = 0; i < quantidade_de_recursos; i++){
            cout << " " << recursos[i];
        }
        cout << endl;


        *tempo_restante -= execucao;
        (*process)[pos].estado = "Executando";
        for(int i = 0; i < execucao * 1000000; i++); //simula o Sleep

        mem_comp(*process, *escalona, pos);
        //pipe(process, escalona, pos);

        //parte que envolve a memória do nosso processo
        cout << "Acessando memoria" << endl;
        acessa_memoria(process, pos, *tempo_restante);
        cout << "Adicionando pagina para o processo " << (*process)[pos].id << endl;
        //adiciona_pg(process, pos, (int)(*process)[pos].Tabela_paginacao.size() - 1, *tempo_restante, false);

        *tempo_atual += execucao;
        cout << endl << endl;
    }
    else{
        cout << "Processo " << (*process)[pos].id << " nao conseguiu alocar recurso" << endl;
    }
    //liberado = recursos_encerrados(process, execucao, pos);
    cout << "Vetor de alocados do Processo " << (*process)[pos].id << ":";
    for(int i = 0; i < quantidade_de_recursos; i++){
        cout << " " << (*process)[pos].alocado[i];
    }
    cout << endl;
}

vector<processos> RR(vector<processos> &programa, int quantum){
    int tempo_atual = 0, aux, pos = 0, flag = 1;
    escalonador escalona(programa.size(), programa);
    vector<processos> saida;
    vector<processos*> nucleos(quantidade_de_nucleos);

    tempo_atual += programa[pos].start_time;
    escalona.fila.push(&programa[pos]);
    programa[pos].estado = "Pronto";
    cout << "Processo " << programa[pos++].id << " esta Pronto...\n";

    while(!escalona.fila.empty() || pos != (int) programa.size()){

        aux = escalona.fila.front()->id - 1;
        escalona.fila.pop();
        executa(quantum, &escalona.tempo_restante[aux], &programa, &escalona, aux, &tempo_atual);

        if(escalona.tempo_restante[aux] == 0){
            programa[aux].end_time = tempo_atual;
            escalona.tat[aux] = tempo_atual - programa[aux].start_time;
            escalona.wt[aux] = escalona.tat[aux] - programa[aux].duracao;
            for(int i = 0; i < programa[aux].numero_paginas; i++){
                if(programa[aux].Tabela_paginacao.back()->presente){
                    cout << "Removendo pagina " << programa[aux].Tabela_paginacao.back()->id
                    << " do adress: " << programa[aux].Tabela_paginacao.back()->adress / PG_LENGTH << endl;
                    mem_usada ^= (1 << programa[aux].Tabela_paginacao.back()->adress / PG_LENGTH); 
                    
                }
                programa[aux].Tabela_paginacao.pop_back();
            }
            saida.push_back(programa[aux]);
            programa[aux].estado = "Finalizado";
            flag = 0;

            vector<int> liberado = recursos_encerrados(&programa, 0, aux, true, &(escalona.tempo_restante[aux]));
            cout << "Quantidade por recurso liberada do Processo " << (programa)[aux].id << ": ";
            for(int i = 0; i < quantidade_de_recursos; i++){
                cout << " " << liberado[i];
            }
            cout << endl;
        }

        for(int i = pos; i < (int)programa.size(); ++i){
            if(programa[i].start_time <= tempo_atual){
                escalona.fila.push(&programa[i]);
                programa[i].estado = "Pronto";
                cout << "Processo " << programa[i].id << " esta Pronto...\n";
                pos++;
            }
            else{
                if(escalona.fila.empty() && !flag) tempo_atual = programa[i--].start_time;
                else break;
            }
        }

        if(flag){
            escalona.fila.push(&programa[aux]);
            programa[aux].estado = "Suspenso";
            saida.push_back(programa[aux]);
        }
        else flag = 1;
        
    }

    cout << endl << "   Resultados: " << endl;
    cout << "-----------------------------" << endl;
    cout << "Tempo levado para executar o programa: " << tempo_atual << endl;
    for(int i = 0; i < (int)programa.size(); ++i){
        cout << "Waiting: "<< escalona.wt[i] << " || " << "Tat: " << escalona.tat[i] << endl;
    }
    return saida;
} 

