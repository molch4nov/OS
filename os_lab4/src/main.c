#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "shrmem.h"
#define max_filename_size 10

int main() {
	int fd = shm_open(BackingFile, O_CREAT | O_RDWR, AccessPerms);

	printf("Enter filename\n");
    char *filename = (char *)malloc(sizeof(char) * max_filename_size);
    scanf("%s", filename);
	int file = open(filename, O_RDONLY);

	if (fd == -1 || file == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }


	sem_t *semptr = sem_open(SemaphoreName, O_CREAT, AccessPerms, 1);
    if (semptr == SEM_FAILED){
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    int val;

	ftruncate(fd, map_size);

	char* memptr = mmap(
	  	NULL,
	  	map_size,
	  	PROT_READ | PROT_WRITE,
	  	MAP_SHARED,
	  	fd,
	  	0);

	if(memptr == MAP_FAILED){
        perror("mmap");
        exit(EXIT_FAILURE);
    }

	if(sem_getvalue(semptr, &val) != 0){
        perror("sem_getvalue");
        exit(EXIT_FAILURE);
    }

    memset(memptr, '\0', map_size);

    while (val-- > 1) {
		sem_wait(semptr);
  	}
  	while (val++ < 0) {
		sem_post(semptr);
  	}

	pid_t pid = fork();
	if(pid == 0){
		if (dup2(file, fileno(stdin)) == -1) {
	  		perror("DUP2");
	  		exit(EXIT_FAILURE);
		}
		execl("child", "child", NULL);
	}
	else if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

	while (true) {
	if (sem_getvalue(semptr, &val) != 0) {
	  perror("SEM_GETVALUE");
	  exit(EXIT_FAILURE);
	}
	if (val == 0) {
	  if (sem_wait(semptr) != 0) {
		perror("SEM_WAIT");
		exit(EXIT_FAILURE);
	  }
	  printf("%s", memptr);
	  memset(memptr, '\0', map_size);
	  if (sem_post(semptr) != 0) {
		perror("SEM_POST");
		exit(EXIT_FAILURE);
	  }
	} else {
	  if (sem_wait(semptr) != 0) {
		perror("SEM_WAIT");
		exit(EXIT_FAILURE);
	  }
	  if (memptr[0] == EOF) {
		break;
	  }
	  if (sem_post(semptr) != 0) {
		perror("SEM_POST");
		exit(EXIT_FAILURE);
	  }
	}
  }

	close(file);
	free(filename);
	if(munmap(memptr, map_size) != 0){
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    close(fd);
    if(sem_close(semptr) != 0){
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    if(shm_unlink(BackingFile) != 0){
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
	
  
	return EXIT_SUCCESS;
}
