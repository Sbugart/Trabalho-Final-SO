#include "Scheduler.h"

const int MEM_LENGTH = 100;
const int PG_LENGTH = 4000; 
const int quantidade_de_recursos = 4;
const int quantidade_de_nucleos = 2;
int mem_usada = 0; 
vector<int> Memoria_Principal(24000);
vector<int> Memoria_Secundaria;
vector<int> recursos;

int main(){

    vector<processos> saida;
    int quantum = 5, quantidade_processos = 3;
    srand(time(0));
    define_espaco_recursos(&recursos, 13);
    cout << "As quantidades por recurso sao: ( " << recursos[0];
    for(int i = 1; i < (int)quantidade_de_recursos; i++) cout << " , " << recursos[i];
    cout << " )" << endl;

    vector<processos> init = inicia_programa(quantidade_processos);

    for(int i = 0; i < quantidade_processos; i++){
        cout << "Processo " << init[i].id << " foi inicializado com:" << endl;
        cout << "start time = " << init[i].start_time << "  e  duracao = " << init[i].duracao << endl << endl;
    }

    saida = RR(init, quantum);

    for(int i = 0; i < quantidade_processos; i++){
        cout << "Processo " << init[i].id << " finalizou em " << init[i].end_time << endl;
    }

    cout << endl << "A ordem em que os processos foram executados foi: " << endl;
    for(const auto& processo : saida){
        cout << "P" << processo.id << " ";
    }
    cout << endl;
}

