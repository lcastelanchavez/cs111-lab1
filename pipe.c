#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

//int errno; 
int main(int argc, char *argv[])
{
	// TODO: it's all yours

	if(argc < 2) //if no args exit with errno EINVAL
	{
		fprintf(stderr, "No args specified \n"); 
		exit(EINVAL); //exit with error invalid argument 
	}



	//create pipe array
	//pipe the array
	//execute or argv[1]
	//pass output/write to pipe (how do we know how much is written?)
	//i think we might have to reverse; argv[1] is child of argv[n-1]
	//therefore, all parents (later args) wait for first
	//might have to do recursion

	//create file I/O descriptor arrays to store output between 
	//processes
	int fds_prev[2];
	int fds_next[2]; 
	
	//loop through all arguments
	for(int i=1; i < argc; i++) {
		
	 
		//will pipe until hits last argument/process
		if(i < argc-1) {
			pipe(fds_next);
		}
		//create parent/child process
		int return_code = fork(); 

		//if in parent process 
		if(return_code > 0) {

			//if processes exist before it
			if(i != 1) 
			{
				//error checking
				if(close(fds_prev[0]) || close(fds_prev[1]) == -1 ) 
				{
					perror("Error in closing parent process: closing files");
					return errno; 
				}

			}
			
			//if first or processes exist after it, i.e. more command line args
			//copy file descriptors over to save them for child's input 
			if(i < argc - 1) {
				fds_prev[0] = fds_next[0];
				fds_prev[1] = fds_next[1]; 

			}


			//wait for child
			int pid = return_code; //until child w/ this pid returns, wait
			int status = 0;
			waitpid(pid, &status,0); 

			printf("child exits with code %d\n", WEXITSTATUS(status)); 
			if( WEXITSTATUS(status) != 0) 
			{
				printf("Child failed!");
				exit(errno); //??? how to exit with proper code
			}
			
		}

		else if(return_code == 0) {
			//printf("This is child process!\n");
			execlp(argv[1], argv[1], NULL); 

			//child passes output to buffer pipe
			//how do we count bytes performed by ls, cat and wc
			//write(fds[1], "", );
			perror("execlp");
			return 1;
			exit(0); //child exits
		}


		else {
			printf("Child process creation error!\n"); 
		}
	
	
	
	}
	

	printf("Finished!\n"); 
	return 0;
}
