#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <fcntl.h>
using namespace std;

int main() {
    string s;                                  
    cout << "Enter a filename with tests:";     //ввод название и перевод к массиву чаров
    cin >> s;
    const char * buf = s.c_str();

    if(access(buf, R_OK) == -1){//проверка имеет ли вызвавший процесс права доступа к buf на запись
        cout << "File not exist" << endl;
        exit(EXIT_FAILURE);
    }

    int fd[2]; 
    pipe(fd);   //создание канала чтения и записи
    

    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        return -1;
    }
    else if(pid == 0){
        int file = open(buf, O_RDONLY);
        if (file == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd[1], 1) == -1) {
            exit(1);
        }
        if (dup2(file, 0) == -1) {
            exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        close(file);
        execl("child", "child", NULL);
    }
    close(fd[1]);
    char c;
    while(read(fd[0], &c, sizeof(char)) > 0){
        putchar(c);
    }
    close(fd[0]);
    return 0;

}