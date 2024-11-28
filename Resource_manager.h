#ifndef _Resource_manager_h_
#define _Resource_manager_h_

bool safety(vector<processos> *programa);
bool novo_recurso(vector<processos> *programa, int time, processos *process);
void marca_solicitacoes(vector<processos> *programa, int tempo_atual, int execucao, int id);
vector<int> recursos_encerrados(vector<processos> *programa, int execucao, int id, bool fim_de_processo, int *tempo_process);

#endif