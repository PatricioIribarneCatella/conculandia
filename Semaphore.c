#include "Semaphore.h"

int Semaphore_init(Semaphore *S, const char *filename, int init_val){
    //Creo la key
    key_t key = ftok(filename, DEFAULT_NUM);
    if (key == ERROR_FTOK) {
        return ERROR_FTOK;
    }

    S->id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (S->id == ERROR_SEMGET) {
        return ERROR_SEMGET;
    }

    S->init_val = init_val;
    union semun dummy;
    dummy.val = S->init_val;

    return semctl(S->id, 0 , SETVAL, dummy);
}

int Semaphore_v(Semaphore *S){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1; //incremento 1
    op.sem_flg = SEM_UNDO;
    return semop(S->id, &op, 1);
}

int Semaphore_p(Semaphore *S){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1; //decremento 1
    op.sem_flg = SEM_UNDO;
    return semop(S->id, &op, 1);
}

int Semaphore_eliminar(Semaphore *S){
    return semctl(S->id, 0, IPC_RMID);
}