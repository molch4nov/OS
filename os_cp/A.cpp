#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
using namespace std;


int quick_get(sem_t *semaphore){
    int s;
    sem_getvalue(semaphore, &s);
    return s;
}

void quick_set(sem_t *semaphore, int n){ //переделать{
    while (quick_get(semaphore) < n){
        sem_post(semaphore);
    }
    while (quick_get(semaphore) > n){
        sem_wait(semaphore);
    }
}

int main(){
    int fdAC[2];
    int fdAB[2];
    int fdBC[2];
    pipe(fdAC);
    pipe(fdAB);
    pipe(fdBC);
    sem_unlink("_semA");
    sem_unlink("_semB");
    sem_unlink("_semC");
    sem_t* semA = sem_open("_semA", O_CREAT, 0777, 1); //0777 разобраться
    sem_t* semB = sem_open("_semB", O_CREAT, 0777, 0);
    sem_t* semC = sem_open("_semC", O_CREAT, 0777, 0);
    if ((semA == SEM_FAILED)||(semB == SEM_FAILED)||(semC == SEM_FAILED)){
        perror("sem_open");
        return -1;
    }
    cout << "Enter some strings:\n";
    pid_t C = fork();
    if (C == -1){
        perror("fork");
        return -1;
    }
    if (C == 0){
        pid_t B = fork();
        if (B == -1){
            perror("fork");
            return -1;
        }
        if (B == 0){
            execl("B", to_string(fdAB[0]).c_str(), to_string(fdAB[1]).c_str(), to_string(fdBC[0]).c_str(), to_string(fdBC[1]).c_str(), NULL); //разобраться
        }
        else{
            execl("C", to_string(fdAC[0]).c_str(), to_string(fdAC[1]).c_str(), to_string(fdBC[0]).c_str(), to_string(fdBC[1]).c_str(), NULL);
        }
    }
    else{
       while(1){
        string str;
        getline(cin, str);
        if (str == "EXIT"){
            quick_set(semA, 2);
            quick_set(semB, 2);
            quick_set(semC, 2);
            break;
        }
        int size = str.length();
        write(fdAC[1], &size, sizeof(int));
        write(fdAB[1], &size, sizeof(int));
        for (int i = 0; i < size; ++i){
            write(fdAC[1], &str[i], sizeof(char));
        }
        quick_set(semB, 1);
        quick_set(semA, 0);
        while (quick_get(semA) == 0){
            continue;
        }
    }
    }
    sem_close(semA);
    sem_destroy(semA);
    sem_close(semB);
    sem_destroy(semB);
    sem_close(semC);
    sem_destroy(semC);
    close(fdAC[0]);
    close(fdAC[1]);
    close(fdAB[0]);
    close(fdAB[1]);
    return 0;
}