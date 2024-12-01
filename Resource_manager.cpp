#include "Resource_manager.h"
#include "OS.h"
#include "Process_manager.h"
#include "Scheduler.h"

bool safety(SistemaOperacional &SO){
    vector<bool> finish(SO.programa.size(), false);
    vector<int> work = SO.recursos; // Copia os recursos disponíveis.

    for (size_t i = 0; i < SO.programa.size(); ++i) {
        bool encontrou = false;
        for (size_t pos = 0; pos < SO.programa.size(); ++pos) {
            if (!finish[pos]) {
                bool suficiente = true;
                for (int recurso = 0; recurso < SO.quantidade_de_recursos; ++recurso) {
                    if (SO.programa[pos].recurso.precisa[recurso] > work[recurso]) {
                        suficiente = false;
                        break;
                    }
                }
                if (suficiente) {
                    for (int recurso = 0; recurso < SO.quantidade_de_recursos; ++recurso) {
                        work[recurso] += SO.programa[pos].recurso.alocado[recurso];
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

bool novo_recurso(SistemaOperacional &SO, processos *process){
    vector<int> alocado_parcialmente(SO.quantidade_de_recursos, 0);

    // Tenta alocar recursos solicitados.
    for (int recurso = 0; recurso < SO.quantidade_de_recursos; ++recurso) {
        if ((*process).recurso.solicitados[recurso] > (*process).recurso.precisa[recurso]) {
            return false; // Solicitação inválida.
        }
        alocado_parcialmente[recurso] = min((*process).recurso.solicitados[recurso], SO.recursos[recurso]);
        (*process).recurso.solicitados[recurso] -= alocado_parcialmente[recurso];
        SO.recursos[recurso] -= alocado_parcialmente[recurso];
        (*process).recurso.alocado[recurso] += alocado_parcialmente[recurso];
        (*process).recurso.precisa[recurso] -= alocado_parcialmente[recurso];
    }

    // Verifica se o sistema está em um estado seguro.
    if (safety(SO)) {
        return true; // Alocação foi bem-sucedida.
    } else {
        // Rollback da alocação.
        for (int recurso = 0; recurso < SO.quantidade_de_recursos; ++recurso) {
            SO.recursos[recurso] += alocado_parcialmente[recurso];
            (*process).recurso.alocado[recurso] -= alocado_parcialmente[recurso];
            (*process).recurso.precisa[recurso] += alocado_parcialmente[recurso];
            (*process).recurso.solicitados[recurso] += alocado_parcialmente[recurso];
        }
        return false;
    }
}


void marca_solicitacoes(SistemaOperacional &SO, int execucao,int id){
    for (auto& chamada : SO.programa[id].recurso.tempo_recursos) {
        if (chamada.inicio_definitivo == -1) {
            if (chamada.inicio_esperado <= SO.tempo_atual + execucao) {
                SO.programa[id].recurso.solicitados[chamada.recurso]++;
                chamada.inicio_definitivo = -2;
            } 
            else break;
        }
    }
}

vector<int> recursos_encerrados(SistemaOperacional &SO, int execucao, int id, bool fim_de_processo){
    vector<int> liberado(SO.quantidade_de_recursos, 0);
    int tempo_extra = 0;
    for (auto& chamada : SO.programa[id].recurso.tempo_recursos) {
        if (chamada.inicio_definitivo >= 0) {
            if (chamada.duracao <= chamada.inicio_definitivo + execucao) {
                SO.recursos[chamada.recurso]++;
                SO.programa[id].recurso.alocado[chamada.recurso]--;
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
    SO.escalona.tempo_restante[id] += tempo_extra;
    return liberado;
}

void define_espaco_recursos(vector<int> *recurso, int max, SistemaOperacional &SO){
    if(max == 4) srand(time(0));
    for(int i = 0; i < (int)SO.quantidade_de_recursos; i++){
        (*recurso).push_back(3 + rand() % max);
        if((*recurso)[i] > SO.recursos[i])((*recurso)[i] = SO.recursos[i]);
    }
}

void inicializa_recursos_processos(processos *novo, SistemaOperacional &SO){
    define_espaco_recursos(&(*novo).recurso.recursos, 7, SO);
    cout << "Quantidade por recurso utilizada do processo eh: ( " << (*novo).recurso.recursos[0];
    for(int i = 0; i < (int)SO.quantidade_de_recursos; i++){
        define_tempo_recursos(&(*novo).recurso.tempo_recursos, i, (*novo).recurso.recursos[i],(*novo).duracao, (*novo).start_time);
        (*novo).recurso.alocado.push_back(0);
        (*novo).recurso.solicitados.push_back(0); 
        (*novo).recurso.precisa.push_back((*novo).recurso.recursos[i] - (*novo).recurso.alocado.back());
        if(i != 0) cout << " , " << (*novo).recurso.recursos[i];
    }
    cout << " )" << endl;
    sort((*novo).recurso.tempo_recursos.begin(), (*novo).recurso.tempo_recursos.end(), [] (tempo_recurso a, tempo_recurso b) {
        return a.inicio_esperado < b.inicio_esperado;
    });
}

void define_tempo_recursos(vector<tempo_recurso> *tempos, int recurso, int quant_a_alocar,int duracao_process, int inicio_process){
    tempo_recurso aux;
    aux.recurso = recurso;
    while(quant_a_alocar != 0){
        aux.inicio_definitivo = -1;
        aux.inicio_esperado = inicio_process + rand() % (duracao_process);
        int tempo_que_ele_pode_rodar = duracao_process - (aux.inicio_esperado - inicio_process);
        aux.duracao = rand() % ((tempo_que_ele_pode_rodar) / 3 + 2);
        (*tempos).push_back(aux);
        quant_a_alocar--;
    }   
}

void atualizaInicioDefinitivo(SistemaOperacional &SO, int pos, int tempo_execucao) {
    cout << "Processo " << SO.programa[pos].id << " alocando recurso " << endl;
    for(auto& recursos : SO.programa[pos].recurso.tempo_recursos){
        if(recursos.inicio_definitivo == -2){
            if(recursos.inicio_esperado <= SO.tempo_atual) recursos.inicio_definitivo = tempo_execucao;
            else recursos.inicio_definitivo = SO.tempo_atual + tempo_execucao - recursos.inicio_esperado;
        }
    }
}