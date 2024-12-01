#include "Scheduler.h"
#include "OS.h"
#include "Resource_manager.h"
#include "Process_manager.h"
#include "Paging.h"
#include "Mensage_manager.h"

void RR(SistemaOperacional &SO){
    int aux, pos = 0, flag = 1;
    
    // Inicializa o tempo do sistema com o tempo de início do primeiro processo
    SO.tempo_atual += SO.programa[pos].start_time;

    // Coloca o primeiro processo na fila de pronto
    SO.escalona.fila.push(&SO.programa[pos]);
    SO.programa[pos].estado = "Pronto";
    
    cout << "Processo " << SO.programa[pos++].id << " esta Pronto...\n";

    // Enquanto houver processos na fila ou processos a serem adicionados
    while(!SO.escalona.fila.empty() || pos != (int) SO.programa.size()){
        aux = SO.escalona.fila.front()->id - 1;  // Obtém o índice do processo
        SO.escalona.fila.pop();  // Remove o processo da fila

        executa(SO, pos);  // Executa o processo

        // Se o tempo restante do processo for zero, finaliza o processo
        if(SO.escalona.tempo_restante[aux] == 0) finaliza_process(SO, flag, aux);
        
        // Adiciona novos processos à fila conforme o tempo atual
        adiciona_process_na_fila(SO, pos, flag);

        // Suspende o processo se necessário
        if(flag) suspende_process(SO, aux);
        else flag = 1;
    }

    // Imprime os resultados após a execução dos processos
    cout << endl << "   Resultados: " << endl;
    cout << "-----------------------------" << endl;
    cout << "Tempo levado para executar o programa: " << SO.tempo_atual << endl;
    for(int i = 0; i < (int)SO.programa.size(); ++i){
        cout << "Waiting: "<< SO.escalona.wt[i] << " || " << "Tat: " << SO.escalona.tat[i] << endl;
    }
} 

void adiciona_process_na_fila(SistemaOperacional &SO, int &pos, int flag){
    for(int i = pos; i < (int)SO.programa.size(); ++i){
        // Se o tempo de início do processo for menor ou igual ao tempo atual
        if(SO.programa[i].start_time <= SO.tempo_atual){
            SO.escalona.fila.push(&SO.programa[i]);  // Adiciona o processo à fila de pronto
            SO.programa[i].estado = "Pronto";  // Define o estado como "Pronto"
            cout << "Processo " << SO.programa[i].id << " esta Pronto...\n";
            pos++;
        }
        else{
            // Se a fila estiver vazia e o flag não estiver ativo, ajusta o tempo atual
            if(SO.escalona.fila.empty() && !flag) SO.tempo_atual = SO.programa[i--].start_time;
            else break;
        }
    }
}

void executa(SistemaOperacional &SO, int pos){
    // Determina o tempo de execução baseado no quantum ou no tempo restante
    int execucao = min(SO.quantum, SO.escalona.tempo_restante[pos]);

    // Obtém os recursos liberados após a execução
    vector<int> liberado = recursos_encerrados(SO, execucao, pos, false);
    
    // Imprime a quantidade de recursos liberados
    print("Quantidade por recurso liberada do Processo ", liberado, SO.programa[pos].id);

    // Marca as solicitações de recursos do processo
    marca_solicitacoes(SO, execucao, pos);

    // Imprime as solicitações de recursos
    print("Vetor de solicitados do Processo ", SO.programa[pos].recurso.solicitados, SO.programa[pos].id );

    // Se o processo precisa de novos recursos, tenta alocá-los
    if (novo_recurso(SO, &(SO.programa[pos]))){
        // Atualiza o início definitivo do processo com o tempo de execução
        atualizaInicioDefinitivo(SO, pos, execucao);

        // Imprime o estado atual dos recursos
        print("Recursos restantes:", SO.recursos, 0);

        // Atualiza o tempo restante do processo
        SO.escalona.tempo_restante[pos] -= execucao;
        SO.programa[pos].estado = "Executando";  // Marca o processo como "Executando"

        // Simula o tempo de execução (equivalente a um "sleep")
        for(int i = 0; i < execucao * 1000000; i++);

        // Realiza operações de memória durante a execução do processo
        mem_comp(SO, pos);
        cout << "Acessando memoria" << endl;
        acessa_memoria(SO, pos);
        cout << "Adicionando pagina para o processo " << SO.programa[pos].id << endl;

        // Atualiza o tempo atual após a execução
        SO.tempo_atual += execucao;
        cout << endl << endl;
    }
    else{
        // Se o processo não conseguiu alocar os recursos, exibe uma mensagem
        cout << "Processo " << SO.programa[pos].id << " nao conseguiu alocar recurso" << endl;
    }

    // Imprime os recursos alocados ao processo
    print("Vetor de alocados do Processo ", SO.programa[pos].recurso.alocado, SO.programa[pos].id);
}
