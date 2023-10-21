#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int errno; 
int main(int argc, char *argv[])
{
	// TODO: it's all yours

	if(argc == 0) //if no args exit with errno EINVAL
	{
		errno = 22; 
		fprintf(stderr,"errno %di\n", errno);
		printf("No args specified \n"); 
		exit(EXIT_FAILURE); 
	}


		


	printf("how many args are there: %d\n", argc-1);
	printf("what they are:\n");
	for(int i=1; i <argc; i++) {
		printf("argv %d is: %s \n", i, argv[i]);
	}


	//Need a way to vet the arguments typed by user are
	//valid args? How could we check this


	int pipefd[2]; 

	//create pipe array
	//pipe the array
	//execute or argv[1]
	//pass output/write to pipe (how do we know how much is written?)
	//i think we might have to reverse; argv[1] is child of argv[n-1]
	//therefore, all parents (later args) wait for first
	//might have to do recursion

	int return_code = fork(); 

	if(return_code == 0) {
		printf("This is child process!\n");
		execlp(argv[1], argv[1], NULL); //pass in child argument based on input args
		//how to pass in diff args based on varying arguements?
		//how to handle flag arguments? are we supposed to

		exit(0); //child exits
	}
	else if(return_code > 0) {
		printf("In parent process rn: \n");
		int pid = return_code; //until child w/ this pid returns, wait
		int status = 0;
		waitpid(pid, &status,0); 

		printf("child exits with code %d\n", WEXITSTATUS(status)); 
	}

	else {
		printf("Child process creation error!\n"); 
	}

	printf("Finished!\n"); 
	return 0;
}
