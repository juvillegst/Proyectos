#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    bool is_completed;
} Process;

// Prototipos de funcionesc
void inputProcesses(Process p[], int n);
void resetProcesses(Process p[], int n);
void printResults(Process p[], int n);

void fcfs(Process p[], int n);
void sjf(Process p[], int n);
void srtf(Process p[], int n);
void roundRobin(Process p[], int n, int quantum);

int main() {
    int n, choice, quantum;

    printf("===================================================\n");
    printf("   SIMULADOR DE ALGORITMOS DE PLANIFICACION CPU    \n");
    printf("===================================================\n");
    
    printf("Ingrese el numero total de procesos: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
        printf("Numero de procesos invalido.\n");
        return 1;
    }

    Process processes[MAX_PROCESSES];
    inputProcesses(processes, n);

    do {
        printf("\n---------------------------------------------------\n");
        printf("SELECCIONE EL ALGORITMO DE PLANIFICACION:\n");
        printf("1. FCFS (First-Come, First-Served)\n");
        printf("2. SJF (Shortest Job First - No apropiativo)\n");
        printf("3. SRTF (Shortest Remaining Time First - Apropiativo)\n");
        printf("4. Round Robin (RR)\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &choice);

        resetProcesses(processes, n);

        switch (choice) {
            case 1:
                printf("\n>>> Ejecutando FCFS <<<\n");
                fcfs(processes, n);
                printResults(processes, n);
                break;
            case 2:
                printf("\n>>> Ejecutando SJF (No apropiativo) <<<\n");
                sjf(processes, n);
                printResults(processes, n);
                break;
            case 3:
                printf("\n>>> Ejecutando SRTF (Apropiativo) <<<\n");
                srtf(processes, n);
                printResults(processes, n);
                break;
            case 4:
                printf("Ingrese la magnitud del Quantum: ");
                scanf("%d", &quantum);
                if (quantum <= 0) {
                    printf("El quantum debe ser mayor a 0.\n");
                    break;
                }
                printf("\n>>> Ejecutando Round Robin (Quantum = %d) <<<\n", quantum);
                roundRobin(processes, n, quantum);
                printResults(processes, n);
                break;
            case 5:
                printf("Cerrando el simulador...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (choice != 5);

    return 0;
}

// Carga de procesos con tiempos de llegada y ráfagas
void inputProcesses(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("\n--- Proceso P%d ---\n", p[i].id);
        printf("  Tiempo de llegada (Arrival Time): ");
        scanf("%d", &p[i].arrival_time);
        printf("  Tiempo de rafaga (Burst Time): ");
        scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
        p[i].is_completed = false;
    }
}

// Restablece variables de cálculo antes de ejecutar otro algoritmo
void resetProcesses(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].turnaround_time = 0;
        p[i].waiting_time = 0;
        p[i].is_completed = false;
    }
}

// Algoritmo First-Come, First-Served (FCFS)
void fcfs(Process p[], int n) {
    int current_time = 0;
    int completed = 0;
    bool visited[MAX_PROCESSES] = {false};

    while (completed < n) {
        int idx = -1;
        int min_arrival = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time < min_arrival) {
                min_arrival = p[i].arrival_time;
                idx = i;
            }
        }

        if (idx != -1) {
            if (current_time < p[idx].arrival_time) {
                current_time = p[idx].arrival_time; // CPU en espera (Idle)
            }
            current_time += p[idx].burst_time;
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            visited[idx] = true;
            completed++;
        }
    }
}

// Algoritmo Shortest Job First (SJF) - No Apropiativo
void sjf(Process p[], int n) {
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !p[i].is_completed) {
                if (p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    idx = i;
                } else if (p[i].burst_time == min_burst) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            current_time += p[idx].burst_time;
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            p[idx].is_completed = true;
            completed++;
        } else {
            current_time++;
        }
    }
}

// Algoritmo Shortest Remaining Time First (SRTF) - Apropiativo
void srtf(Process p[], int n) {
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int min_remaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !p[i].is_completed) {
                if (p[i].remaining_time < min_remaining && p[i].remaining_time > 0) {
                    min_remaining = p[i].remaining_time;
                    idx = i;
                } else if (p[i].remaining_time == min_remaining && p[i].remaining_time > 0) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            p[idx].remaining_time--;
            current_time++;

            if (p[idx].remaining_time == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                p[idx].is_completed = true;
                completed++;
            }
        } else {
            current_time++;
        }
    }
}

// Algoritmo Round Robin (RR)
void roundRobin(Process p[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    int queue[1000];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};

    // Encuentra el primer tiempo de llegada
    int min_arrival = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time < min_arrival) min_arrival = p[i].arrival_time;
    }
    current_time = min_arrival;

    // Encola procesos iniciales
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time == current_time) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }

    while (completed < n) {
        if (front == rear) { // Si la cola está vacía, avanza al siguiente proceso que llegue
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!p[i].is_completed && p[i].arrival_time > current_time) {
                    if (p[i].arrival_time < next_arrival) {
                        next_arrival = p[i].arrival_time;
                    }
                }
            }
            current_time = next_arrival;
            for (int i = 0; i < n; i++) {
                if (!p[i].is_completed && p[i].arrival_time == current_time && !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }
        }

        int idx = queue[front++];
        int execute_time = (p[idx].remaining_time < quantum) ? p[idx].remaining_time : quantum;
        
        p[idx].remaining_time -= execute_time;
        current_time += execute_time;

        // Añade a la cola los nuevos procesos que hayan llegado durante este intervalo
        for (int i = 0; i < n; i++) {
            if (i != idx && !p[i].is_completed && !in_queue[i] && p[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        if (p[idx].remaining_time == 0) {
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            p[idx].is_completed = true;
            completed++;
        } else {
            // Se reencola el proceso incompleto al final
            queue[rear++] = idx;
        }
    }
}

// Presentación de la tabla de resultados y promedios
void printResults(Process p[], int n) {
    float total_wt = 0, total_tat = 0;

    printf("\n+---------+----------------+---------------+---------------+----------------+----------------+\n");
    printf("| Proceso | Tiempo Llegada | Tiempo Rafaga | Finalizacion  | T. Retorno      | T. Espera      |\n");
    printf("+---------+----------------+---------------+---------------+----------------+----------------+\n");

    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        printf("| P%-6d | %-14d | %-13d | %-13d | %-14d | %-14d |\n",
               p[i].id, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
    printf("+---------+----------------+---------------+---------------+----------------+----------------+\n");

    printf("\nTiempo Medio de Retorno (Average TAT): %.2f u.t.\n", total_tat / n);
    printf("Tiempo Medio de Espera (Average WT):    %.2f u.t.\n", total_wt / n);
}