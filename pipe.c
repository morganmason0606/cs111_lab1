
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

	int fds[argc][2];
	pid_t cpid;
	int wstatus;

	if(argc == 1){
		exit(EINVAL);
	}

	for(int i = 1; i < argc; i++){
		
		if(pipe(fds[i]) == -1){
			perror("pipe() failure");
			exit(EXIT_FAILURE);
		}

		cpid = fork();
		if(cpid == -1){
			perror("fork() failure");
			exit(EXIT_FAILURE);
		}else if(cpid == 0){
			
			if(i != argc - 1){
				dup2(fds[i][1],STDOUT_FILENO);
			}
			
			if(i != 1){
				dup2(fds[i-1][0],STDIN_FILENO);
			}
			
			close(fds[i][1]);
			close(fds[i][0]);

			execlp(argv[i],argv[i],NULL);
			exit(EXIT_FAILURE);
		}else{
			close(fds[i][1]);
			wait(&wstatus);
			if(wstatus != 0){
				exit(EXIT_FAILURE);
			}
		}
	}

	return 0; 
}


