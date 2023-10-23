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
				if(close(fds_prev[0]) == -1 ) 
				{
					perror("Error in closing parent process: closing files");
					return errno; 
				}
				if (close(fds_prev[1])== -1)
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

			//printf("child exits with code %d\n", WEXITSTATUS(status)); 
			if( WEXITSTATUS(status) != 0) 
			{
				fprintf(stderr, "Child failed!\n");
				return -1; 
			}
			
		}

		//if in child process 
		else if(return_code == 0) {
			//printf("This is child process!\n");

			//if processes exist before this one and not first 
			if(i != 1)
			{
				//make previous process, the next process stdin
				//essentially make it so we can read from pipe rather than stdin
				if(dup2(fds_prev[0], STDIN_FILENO) == -1)
				{
					printf("Error dup2 stdin in child process in %d argument\n", i);
					return errno; 
				}
				//close fds because resources limited
				if (close(fds_prev[0]) == -1)
                {
                    perror("Error closing fds in child process\n");
                    return errno;
                }
                if (close(fds_prev[1]) == -1)
                {
                    perror("Error closing fds in child process\n");
                    return errno;
                }

			}

			//if process after this one exist; need to pass output to pipe
			if (i < argc-1) 
			{
				
				//redirect std output of process to pipe array
				//make it so output goes to pipe rather than stdout
                if (dup2(fds_next[1], STDOUT_FILENO) == -1)
                {
                    printf("Error dup2 stdout in child process in %d argument\n", i);
                    return errno;
                };
				//close fds 
				if (close(fds_next[0]) == -1)
                {
                    perror("Error in child process\n");
                    return errno;
                };
                if (close(fds_next[1]) == -1)
                {
                    perror("Error in child process\n");
                    return errno;
                };

			}



			// check for exec failure
            if (execlp(argv[i], argv[i], NULL) == -1)
            {
                perror("Error running a process (execlp). \n");
                return errno;
				//exit(0);

            }

		}

		//forking failed
		else {
			fprintf(stderr, "Child process creation error!\n"); 
			return errno; 
		}
	
	
	
	}
	

	//printf("Finished!\n"); 
	return 0;
}
