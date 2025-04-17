#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define NUM_THREADS 99

sem_t mutex;          // Para exclusão mútua
sem_t oxygen_queue;   // Oxigênio esperando
sem_t hydrogen_queue; // Hidrogênio esperando

int oxygen_count = 0; // Oxigênios esperando
int hydrogen_count = 0; // Hidrogênios esperando
int molecules_formed = 0; // Contador de moléculas formadas

void clear_line() {
    printf("\033[2K\r"); // Limpa a linha atual
    fflush(stdout);
}

void print_atom(char type, int id, const char* action) {
    const char* color = (type == 'O') ? "\033[1;34m" : "\033[1;33m";
    printf("%s● %c%d %s\033[0m", color, type, id, action);
    fflush(stdout);
}

void animate_bonding(int id) {
    const char* frames[] = {"⏳", "⚡", "💧"};
    for (int i = 0; i < 3; i++) {
        clear_line();
        printf("\033[1;32mFormando molécula H₂O #%d %s\033[0m", id, frames[i]);
        fflush(stdout);
        usleep(200000);
    }
    clear_line();
    printf("\033[1;32m✔ Molécula H₂O #%d formada!\033[0m\n", id);
}

void bond(char atom_type, int id) {
    print_atom(atom_type, id, "está se ligando...\n");
    usleep(100000);
}

void* oxygen(void* arg) {
    int id = *((int*)arg);
    
    sem_wait(&mutex);
    oxygen_count++;
    print_atom('O', id, "chegou ao pool\n");
    
    if (hydrogen_count >= 2) {
        sem_post(&hydrogen_queue);
        sem_post(&hydrogen_queue);
        hydrogen_count -= 2;
        sem_post(&oxygen_queue);
        oxygen_count--;
        molecules_formed++;
        animate_bonding(molecules_formed);
    } else {
        sem_post(&mutex);
    }
    
    sem_wait(&oxygen_queue);
    bond('O', id);
    
    sem_post(&mutex);
    return NULL;
}

void* hydrogen(void* arg) {
    int id = *((int*)arg);
    
    sem_wait(&mutex);
    hydrogen_count++;
    print_atom('H', id, "chegou ao pool\n");
    
    if (hydrogen_count >= 2 && oxygen_count >= 1) {
        sem_post(&hydrogen_queue);
        sem_post(&hydrogen_queue);
        hydrogen_count -= 2;
        sem_post(&oxygen_queue);
        oxygen_count--;
        molecules_formed++;
        animate_bonding(molecules_formed);
    } else {
        sem_post(&mutex);
    }
    
    sem_wait(&hydrogen_queue);
    bond('H', id);
    
    return NULL;
}

void print_pool_status() {
    printf("\n\033[1;36m═══ Pool de Átomos ═══\033[0m\n");
    
    printf("\033[1;34mO: ");
    for (int i = 0; i < oxygen_count; i++) printf("● ");
    printf("\n\033[0m");
    
    printf("  \033[1;33mH: ");
    for (int i = 0; i < hydrogen_count; i++) printf("● ");
    printf("\n\033[0m\n");
    
    printf("\033[1;35mMoléculas formadas: %d\033[0m\n", molecules_formed);
    
    if (oxygen_count >= 1 && hydrogen_count >= 2) {
        printf("\033[1;32m✔ Pode formar H₂O!\033[0m\n");
    } else {
        printf("\033[1;31m✖ Esperando átomos...\033[0m\n");
    }
    
    printf("\033[1;36m══════════════════════\033[0m\n\n");
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    sem_init(&mutex, 0, 1);
    sem_init(&oxygen_queue, 0, 0);
    sem_init(&hydrogen_queue, 0, 0);
    
    srand(time(NULL));
    
    printf("\033[2J\033[H"); // Limpa a tela
    printf("\033[1;35m=== Simulação de Formação de Água H₂O ===\033[0m\n\n");
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        int type = (rand() % 3 == 1) ? 0 : 1; // 1/3 chance de O, 2/3 de H
        
        if (type == 0) {
            pthread_create(&threads[i], NULL, oxygen, &thread_ids[i]);
        } else {
            pthread_create(&threads[i], NULL, hydrogen, &thread_ids[i]);
        }
        
        print_pool_status();
        usleep(100000 + rand() % 100000);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&mutex);
    sem_destroy(&oxygen_queue);
    sem_destroy(&hydrogen_queue);
    
    printf("\n\033[1;35m=== Simulação concluída! ===\033[0m\n");
    printf("\033[1;32mTotal de moléculas H₂O formadas: %d\033[0m\n", molecules_formed);
    
    return 0;
}