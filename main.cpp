#include "escalonamento.h"

int main(){

    int quantum = 5, quantidade_processos = 3;
    vector<processos> init = inicia_programa(quantidade_processos);

    for(int i = 0; i < quantidade_processos; i++){
        cout << "Processo " << init[i].id << " foi inicializado com:" << endl;
        cout << "start time = " << init[i].start_time << "  e  duracao = " << init[i].duracao << endl << endl;
    }

    RR(init, quantum);

    for(int i = 0; i < quantidade_processos; i++){
        cout << "Processo " << init[i].id << " finalizou em " << init[i].end_time << endl;
    }

}

