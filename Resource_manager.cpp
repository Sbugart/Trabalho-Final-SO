#include "Scheduler.h"

bool safety(vector<processos> *programa) {
    vector<bool> finish((*programa).size(), false);
    vector<int> work = recursos; // Copia os recursos disponíveis.

    for (size_t i = 0; i < (*programa).size(); ++i) {
        bool encontrou = false;
        for (size_t pos = 0; pos < (*programa).size(); ++pos) {
            if (!finish[pos]) {
                bool suficiente = true;
                for (int recurso = 0; recurso < quantidade_de_recursos; ++recurso) {
                    if ((*programa)[pos].precisa[recurso] > work[recurso]) {
                        suficiente = false;
                        break;
                    }
                }
                if (suficiente) {
                    for (int recurso = 0; recurso < quantidade_de_recursos; ++recurso) {
                        work[recurso] += (*programa)[pos].alocado[recurso];
                    }
                    finish[pos] = true;
                    encontrou = true;
                    break;
                }
            }
        }
        if (!encontrou) {
            return false;
        }
    }
    return true;
}

bool novo_recurso(vector<processos> *programa, int time, processos *process) {
    vector<int> alocado_parcialmente(quantidade_de_recursos, 0);

    // Tenta alocar recursos solicitados.
    for (int recurso = 0; recurso < quantidade_de_recursos; ++recurso) {
        if ((*process).solicitados[recurso] > (*process).precisa[recurso]) {
            return false; // Solicitação inválida.
        }
        alocado_parcialmente[recurso] = min((*process).solicitados[recurso], recursos[recurso]);
        (*process).solicitados[recurso] -= alocado_parcialmente[recurso];
        recursos[recurso] -= alocado_parcialmente[recurso];
        (*process).alocado[recurso] += alocado_parcialmente[recurso];
        (*process).precisa[recurso] -= alocado_parcialmente[recurso];
    }

    // Verifica se o sistema está em um estado seguro.
    if (safety(programa)) {
        return true; // Alocação foi bem-sucedida.
    } else {
        // Rollback da alocação.
        for (int recurso = 0; recurso < quantidade_de_recursos; ++recurso) {
            recursos[recurso] += alocado_parcialmente[recurso];
            (*process).alocado[recurso] -= alocado_parcialmente[recurso];
            (*process).precisa[recurso] += alocado_parcialmente[recurso];
            (*process).solicitados[recurso] += alocado_parcialmente[recurso];
        }
        return false;
    }
}


void marca_solicitacoes(vector<processos> *programa, int tempo_atual, int execucao, int id){
    for (auto& chamada : (*programa)[id].tempo_recursos) {
        if (chamada.inicio_definitivo == -1) {
            if (chamada.inicio_esperado <= tempo_atual + execucao) {
                (*programa)[id].solicitados[chamada.recurso]++;
                chamada.inicio_definitivo = -2;
            } 
            else break;
        }
    }
}

vector<int> recursos_encerrados(vector<processos> *programa, int execucao, int id, bool fim_de_processo, int *tempo_process){
    vector<int> liberado(quantidade_de_recursos, 0);
    int tempo_extra = 0;
    for (auto& chamada : (*programa)[id].tempo_recursos) {
        if (chamada.inicio_definitivo >= 0) {
            if (chamada.duracao <= chamada.inicio_definitivo + execucao) {
                recursos[chamada.recurso]++;
                (*programa)[id].alocado[chamada.recurso]--;
                liberado[chamada.recurso]++;
                chamada.inicio_definitivo = -3;
            } 
            else{
                if(!fim_de_processo) chamada.inicio_definitivo += execucao;
                else{
                    if(tempo_extra > chamada.duracao - chamada.inicio_definitivo + execucao){
                        tempo_extra = chamada.duracao - chamada.inicio_definitivo + execucao;
                    }
                }
            } 
        }
        else{
            if(fim_de_processo && chamada.inicio_definitivo == -1 && tempo_extra == 0) tempo_extra++;
        }
    }
    (*tempo_process) += tempo_extra;
    return liberado;
}
