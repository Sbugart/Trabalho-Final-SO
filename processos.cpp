#include "escalonamento.h"

processos cria_process(int duracao, int inicio){
    processos novo(duracao, inicio);
    cout << "Um novo processo foi criado" << endl;
    return novo;
}

vector<processos> inicia_programa(int quantidade_procesoss){
    vector<processos> programa;
    int duracao, inicio;
    srand(time(0));

    for(int i = 0; i < quantidade_procesoss; i++){
        duracao = 1 + rand() % 25;
        inicio = rand() % 20;
        programa.push_back(cria_process(duracao, inicio));
    }

    sort(programa.begin(), programa.end(), [] (processos a, processos b){
        return a.start_time < b.start_time;
    });

    for(int i = 0; i < quantidade_procesoss; i++) programa[i].id = i + 1;
    
    return programa;
}

