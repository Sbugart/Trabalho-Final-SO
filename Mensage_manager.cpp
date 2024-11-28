#include "Mensage_manager.h"

void pipe(vector<processos> &process, escalonador &escalona, int pos){
    string resposta = "Enviando arquivo\n";
    string mensagem = "Processo " + to_string(process[pos].id) + " solicitou arquivo\n";
    
    for(int i = 0; i < process[pos].pipe_messenger.size(); i++){
        if(process[pos].pipe_message.front() != resposta){
            int destino = process[pos].pipe_messenger.front() - 1;
            process[destino].pipe_message.push(resposta);
            process[destino].pipe_messenger.push(process[pos].id);
        }
        cout << "Processo " << process[pos].id << " recebeu a mensagem do processo " << process[process[pos].pipe_messenger.front() - 1].id << endl;
        process[pos].pipe_message.pop();
        process[pos].pipe_messenger.pop();
    }

    queue<processos*> temp_queue = escalona.fila;
    for (int i = 0; i < (int)escalona.fila.size(); i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        (*p).pipe_message.push(mensagem);
        (*p).pipe_messenger.push(process[pos].id);
        cout << "Processo " << process[pos].id << " enviou uma mensagem para " << (*p).id << endl;
    }
}

void enviar_mensagem(processos &process, escalonador &escalona, int &pos, processos &p){
    string mensagem = "Processo " + to_string(process.id) + " solicitou arquivo\n";

    while(pos <= MEM_LENGTH - 1){
        if(escalona.usado[pos] != 0) pos++;
        else break;
    }
    if(pos > MEM_LENGTH - 1){
        cout << "Espaço de memória lotado" << endl;
        return; 
    }
    escalona.memoria_mensagens[pos] = mensagem;
    escalona.usado[pos] = process.id;
    escalona.receber[pos] = p.id;
    cout << "Processo " << process.id << " enviou uma mensagem para " << p.id << endl;
}

void recebe(vector<processos> &process, escalonador &escalona,int pos, int pos_pro){
    string resposta = "Enviando arquivo\n";
    if((escalona).memoria_mensagens[pos].compare(resposta) && 
    ((process)[(escalona).usado[pos] - 1].estado.compare("Finalizado") != 0)){
        (escalona).memoria_mensagens[pos] = resposta;
        cout << "Processo " << (process)[pos_pro].id << " recebeu a mensagem do processo " << (escalona).usado[pos] << endl;
        (escalona).receber[pos] = (escalona).usado[pos];
        (escalona).usado[pos] = (process)[pos_pro].id;
    }
    else{
        (escalona).usado[pos] = 0;
        (escalona).receber[pos] = -1;
    }   
}

void mem_comp(vector<processos> &programa, escalonador &escalona, int process_position){

    int tamanho = (int)escalona.fila.size(), pos = 0;
    queue<processos*> temp_queue = (escalona).fila;
    for(int i = 0; i < MEM_LENGTH; i++){
        if((escalona).receber[i] == (programa)[process_position].id) recebe(programa, escalona, i, process_position);
    }

    for (int i = 0; i < tamanho; i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        enviar_mensagem(programa[process_position], escalona, pos, *p);
    }
}