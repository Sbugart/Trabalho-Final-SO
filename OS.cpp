#include "OS.h"
#include "Scheduler.h"
#include "Process_manager.h"
#include "Resource_manager.h"

int main() {
    // Constantes que definem os parâmetros do sistema
    const int quantidade_processos = 3;  // Quantidade de processos
    const int quantum = 5;               // Quantum do escalonador (tempo máximo de execução por processo)
    const int MEM_LENGTH = 100;          // Tamanho da memória
    const int quantidade_de_recursos = 4; // Número de tipos de recursos disponíveis
    const int max_recursos = 13;         // Quantidade máxima de recursos por tipo
    const int nucleos = 2;               // Número de núcleos do sistema operacional

    // Inicializa o gerador de números aleatórios com o horário atual para gerar valores variáveis
    srand(time(0));

    // Criação do objeto principal que representa o sistema operacional
    SistemaOperacional Sistema_Operacional(
        quantidade_processos, quantum, quantidade_de_recursos, 
        max_recursos, MEM_LENGTH, nucleos);

    // Inicializa os programas que serão executados no sistema operacional
    Sistema_Operacional.programa = inicia_programa(Sistema_Operacional);

    // Define a quantidade de recursos disponíveis no sistema
    define_espaco_recursos(&Sistema_Operacional.recursos, 
                           Sistema_Operacional.max_quant_por_recursos, 
                           Sistema_Operacional);

    // Exibe os recursos disponíveis no sistema
    cout << "As quantidades por recurso sao: ( " << Sistema_Operacional.recursos[0];
    for (int i = 1; i < (int)quantidade_de_recursos; i++) {
        cout << " , " << Sistema_Operacional.recursos[i];
    }
    cout << " )" << endl;

    // Exibe informações de inicialização para cada processo
    for (const auto& processo : Sistema_Operacional.programa) {
        cout << "Processo " << processo.id << " foi inicializado com:" << endl;
        cout << "start time = " << processo.start_time << "  e  duracao = " << processo.duracao << endl << endl;
    }

    // Chama o escalonador Round Robin (RR) para executar os processos
    RR(Sistema_Operacional);

    // Exibe informações de término para cada processo
    for (const auto& processo : Sistema_Operacional.programa) {
        cout << "Processo " << processo.id << " finalizou em " << processo.end_time << endl;
    }

    // Exibe a ordem em que os processos foram executados
    cout << endl << "A ordem em que os processos foram executados foi: " << endl;
    for (const auto& processo : Sistema_Operacional.ordem_de_saida) {
        cout << "P" << processo.id << " ";
    }
    cout << endl;
}
