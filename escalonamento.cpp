#include "escalonamento.h"

void pipe(vector<processos> *process, escalonador *escalona, int pos){
    string resposta = "Enviando arquivo\n";
    string mensagem = "Processo " + to_string((*process)[pos].id) + " solicitou arquivo\n";
    for(int i = 0; i < (*process)[pos].pipe_messenger.size(); i++){
        if((*process)[pos].pipe_message.front().compare(resposta) != 0){
            (*process)[(*process)[pos].pipe_messenger.front() - 1].pipe_message.push(resposta);
            (*process)[(*process)[pos].pipe_messenger.front() - 1].pipe_messenger.push((*process)[pos].id);
        }
        cout << "Processo " << (*process)[pos].id << " recebeu a mensagem do processo " << (*process)[(*process)[pos].pipe_messenger.front() - 1].id << endl;
        (*process)[pos].pipe_message.pop();
        (*process)[pos].pipe_messenger.pop();
    }

    queue<processos*> temp_queue = (*escalona).fila;
    for (int i = 0; i < (int)(*escalona).fila.size(); i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        (*p).pipe_message.push(mensagem);
        (*p).pipe_messenger.push((*process)[pos].id);
        cout << "Processo " << (*process)[pos].id << " enviou uma mensagem para " << (*p).id << endl;
    }
}

void enviar_mensagem(processos *process, escalonador *escalona, int *pos, processos* p){
    string mensagem = "Processo " + to_string((*process).id) + " solicitou arquivo\n";

    while(*pos <= MEM_LENGTH - 1){
        if((*escalona).usado[*pos] != 0) (*pos)++;
        else break;
    }
    if(*pos > MEM_LENGTH - 1){
        cout << "Espaço de memória lotado" << endl;
        return; 
    }
    (*escalona).memoria_mensagens[*pos] = mensagem;
    (*escalona).usado[*pos] = (*process).id;
    (*escalona).receber[*pos] = p->id;
    cout << "Processo " << (*process).id << " enviou uma mensagem para " << p->id << endl;
}

void recebe(vector<processos> *process, escalonador *escalona,int pos, int pos_pro){
    string resposta = "Enviando arquivo\n";
    if((*escalona).memoria_mensagens[pos].compare(resposta) && 
    ((*process)[(*escalona).usado[pos] - 1].estado.compare("Finalizado") != 0)){
        (*escalona).memoria_mensagens[pos] = resposta;
        cout << "Processo " << (*process)[pos_pro].id << " recebeu a mensagem do processo " << (*escalona).usado[pos] << endl;
        (*escalona).receber[pos] = (*escalona).usado[pos];
        (*escalona).usado[pos] = (*process)[pos_pro].id;
    }
    else{
        (*escalona).usado[pos] = 0;
        (*escalona).receber[pos] = -1;
    }   
}

void mem_comp(vector<processos> *process, escalonador *escalona, int pos_pro){
    int tamanho = (int)(*escalona).fila.size(), pos = 0;
    queue<processos*> temp_queue = (*escalona).fila;
    for(int i = 0; i < MEM_LENGTH; i++){
        if((*escalona).receber[i] == (*process)[pos_pro].id) recebe(process, escalona, i, pos_pro);
    }
    for (int i = 0; i < tamanho; i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        enviar_mensagem(&(*process)[pos_pro], escalona, &pos, p);
    }
}

int executa(int quantum, int *tempo_restante, vector<processos> *process, escalonador *escalona, int pos){
    int execucao = min(quantum, *tempo_restante);
    *tempo_restante -= execucao;
    (*process)[pos].estado = "Executando";
    cout << "Processo " << (*process)[pos].id << " está Executando...\n";
    Sleep(execucao * 100);

    //mem_comp(process, escalona, pos);
    pipe(process, escalona, pos);

    //parte que envolve a memória do nosso processo
    cout << "Acessando memoria" << endl;
    acessa_memoria(process, pos, *tempo_restante);
    cout << "Adicionando pagina para o processo " << (*process)[pos].id << endl;
    //adiciona_pg(process, pos, (int)(*process)[pos].Tabela_paginacao.size() - 1, *tempo_restante, false);

    return execucao;
}

void RR(vector<processos> &programa, int quantum){
    int tempo_atual = 0, aux, pos = 0, flag = 1;
    escalonador escalona(programa.size(), programa);

    tempo_atual += programa[pos].start_time;
    escalona.fila.push(&programa[pos]);
    programa[pos].estado = "Pronto";
    cout << "Processo " << programa[pos++].id << " está Pronto...\n";

    while(!escalona.fila.empty() || pos != (int) programa.size()){
        aux = escalona.fila.front()->id - 1;
        escalona.fila.pop();

        tempo_atual += executa(quantum, &escalona.tempo_restante[aux], &programa, &escalona, aux);

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
            programa[aux].estado = "Finalizado";
            cout << "Processo " << programa[aux].id << " está Finalizado...\n";
            flag = 0;
        }

        for(int i = pos; i < (int)programa.size(); ++i){
            if(programa[i].start_time <= tempo_atual){
                escalona.fila.push(&programa[i]);
                programa[i].estado = "Pronto";
                cout << "Processo " << programa[i].id << " está Pronto...\n";
                pos++;
            }
            else{
                if(escalona.fila.empty() && !flag) tempo_atual = programa[i--].start_time;
                //Eu coloquei o flag aqui, pois notei q sem isso ele vai adicionar
                // um novo processo dps do primeiro executar e ainda n voltar para a fila.
                else break;
            }
        }

        if(flag){
            escalona.fila.push(&programa[aux]);
            programa[aux].estado = "Suspenso";
            cout << "Processo " << programa[aux].id << " está Suspenso...\n";
        }
        else flag = 1;
    }

    cout << endl << "   Resultados: " << endl;
    cout << "-----------------------------" << endl;
    cout << "Tempo levado para executar o programa: " << tempo_atual << endl;
    for(int i = 0; i < (int)programa.size(); ++i){
        cout << "Waiting: "<< escalona.wt[i] << " || " << "Tat: " << escalona.tat[i] << endl;
    }
} 
