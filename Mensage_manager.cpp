#include "Mensage_manager.h"
#include "OS.h"
#include "Scheduler.h"
#include "Process_manager.h"

void pipe(vector<processos> &programa, escalonador &escalona, int pos){
    string resposta = "Enviando arquivo\n"; // Mensagem de resposta padrão
    string mensagem = "Processo " + to_string(programa[pos].id) + " solicitou arquivo\n"; // Mensagem enviada pelo processo

    // Percorre todas as mensagens pendentes no pipe do processo
    for (int i = 0; i < (int)programa[pos].process_pipes.pipe_messenger.size(); i++) {
        // Verifica se a mensagem não é a resposta padrão
        if (programa[pos].process_pipes.pipe_message.front() != resposta) {
            // Identifica o destino da mensagem
            int destino = programa[pos].process_pipes.pipe_messenger.front() - 1;
            // Envia a mensagem para o processo destino
            programa[destino].process_pipes.pipe_message.push(resposta);
            programa[destino].process_pipes.pipe_messenger.push(programa[pos].id);
        }

        // Imprime a mensagem recebida
        cout << "Processo " << programa[pos].id << " recebeu a mensagem do processo "
             << programa[programa[pos].process_pipes.pipe_messenger.front() - 1].id << endl;

        // Remove as mensagens processadas da fila
        programa[pos].process_pipes.pipe_message.pop();
        programa[pos].process_pipes.pipe_messenger.pop();
    }

    // Envia a mensagem para todos os processos na fila do escalonador
    queue<processos*> temp_queue = escalona.fila;
    for (int i = 0; i < (int)escalona.fila.size(); i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        (*p).process_pipes.pipe_message.push(mensagem); // Adiciona mensagem à fila do processo
        (*p).process_pipes.pipe_messenger.push(programa[pos].id);
        cout << "Processo " << programa[pos].id << " enviou uma mensagem para " << (*p).id << endl;
    }
}

void enviar_mensagem(processos &process, SistemaOperacional &SO, int &pos, processos &p){
    string mensagem = "Processo " + to_string(process.id) + " solicitou arquivo\n";

     // Busca uma posição livre na memória compartilhada
    while(pos <= SO.MEM_LENGTH - 1){
        if(SO.escalona.usado[pos] != 0) pos++;
        else break;
    }

    // Verifica se a memória está cheia
    if(pos > SO.MEM_LENGTH - 1){
        cout << "Espaço de memória lotado" << endl;
        return; 
    }

    // Adiciona a mensagem na memória compartilhada
    SO.escalona.memoria_mensagens[pos] = mensagem;
    SO.escalona.usado[pos] = process.id;
    SO.escalona.receber[pos] = p.id;
    cout << "Processo " << process.id << " enviou uma mensagem para " 
    << p.id << endl;
}

void recebe(SistemaOperacional &SO ,int pos, int process_position){
    string resposta = "Enviando arquivo\n";

    // Verifica se a mensagem pode ser processada
    if (SO.escalona.memoria_mensagens[pos].compare(resposta) &&
        (SO.programa[SO.escalona.usado[pos] - 1].estado.compare("Finalizado") != 0)) {
        SO.escalona.memoria_mensagens[pos] = resposta;
        cout << "Processo " << SO.programa[process_position].id
             << " recebeu a mensagem do processo " << SO.escalona.usado[pos] << endl;

        // Atualiza os registros da memória compartilhada
        SO.escalona.receber[pos] = SO.escalona.usado[pos];
        SO.escalona.usado[pos] = SO.programa[process_position].id;
    } else {
        // Libera a posição na memória compartilhada
        SO.escalona.usado[pos] = 0;
        SO.escalona.receber[pos] = -1;
    }
}

void mem_comp(SistemaOperacional &SO, int process_position){
    int tamanho = (int)SO.escalona.fila.size(), pos = 0;
    queue<processos*> temp_queue = (SO.escalona).fila;

    // Verifica mensagens para o processo atual
    for (int i = 0; i < SO.MEM_LENGTH; i++) {
        if (SO.escalona.receber[i] == SO.programa[process_position].id) 
            recebe(SO, i, process_position);
    }

    // Envia mensagens para todos os processos na fila
    for (int i = 0; i < tamanho; i++) {
        processos *p = temp_queue.front();
        temp_queue.pop();
        enviar_mensagem(SO.programa[process_position], SO, pos, *p);
    }
}
