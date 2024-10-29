#ifndef _escalonamento_h_
#define _escalonamento_h_

#include "processos.h"

/**
 * @brief Aplicação do escalonador Round Robin.
 * @param programa Lista de processos do programa.
 * @param quantum Inteiro que define o quantum do escalonador.
*/
void RR(vector<processos> &programa, int quantum);

#endif