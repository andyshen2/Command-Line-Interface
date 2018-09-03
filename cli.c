#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void piping(char *c[][20]){
			int p[2];
			pid_t pid;
			int fd = 0;
			int status;
			int i=0;
			
		
			while(*c[0] != NULL){
			
				pipe(p);
				
				if((pid = fork()) == -1){
			
					exit(EXIT_FAILURE);

				}
				//child process
				else if (pid == 0){
					
						//read
						dup2(fd, 0);

						i++;
						//if the next command is a redirect 
						if(*(c +1)[0] != NULL && strcmp(*(c+1)[0], ">") == 0){
							FILE *stream;
							//open a file 
							stream = fopen(c[i][1], "w");	
							//file descriptor 
							int file = fileno(stream);	
								//redirecting standard output
								if(dup2(file, 1) <0){
										
										fclose(stream);
										
								}
							
						}
						//if the array is not null yet
						else if (*(c +1)[0] != NULL){
						
							dup2(p[1], 1);
							
						
						}
							
						//close end of pipe	
						close(p[0]);
					
						execvp(*c[0], *c);
					
						exit(EXIT_FAILURE);
						
				}
				else{
				
					//waits for child
					wait(NULL);
				
					//closes one end of pipe
					close(p[1]);
					//saving input
					fd = p[0];	 
					c++;
					
				}
		}
}
	
int main(int argc, char* argv[], char* envp[])
{
	const char delim[2] = " \n";
	char *token;
	char* bin	= "/bin/";
	char *array[20][20];
	long size;
	char *buf;
	char *ptr;
	char *buffer;
  size_t bufsize = 32;
  size_t characters;
	int j = 0;
	int k = 0;
	int i = 0;


	 buffer = (char *)malloc(bufsize * sizeof(char));
	 if( buffer == NULL)
   {
       perror("Unable to allocate buffer");
       exit(1);
   }

	while(1){
		
		size = pathconf(".", _PC_PATH_MAX);

	 
		if ((buf = (char *)malloc((size_t)size)) != NULL)
		//gets the current directory 
		ptr = getcwd(buf, (size_t)size);

		printf("%s ", ptr);
		//gets a line from the command line
		characters = getline(&buffer,&bufsize,stdin);
		//puts the first command into my 2d array that is split on spaces and new lines
		array[j][k] = token = strtok(buffer, delim);
		
		token = strtok(NULL, delim);
		//increment k for the for any arguments after that command
		k++;

		while(token != NULL) {
			//if the command is a pipe
			if((strcmp(token, "|"))==0){
				token = strtok(NULL, delim);
				//increment j so the new command is on a new col 
				j++;
				array[j][k] = NULL;
				//reset the rows
				k = 0;
			}
			//redirect sysboml
			if((strcmp(token, ">"))==0){
				j++;
			  k = 0;
			}
			//arguments
			array[j][k++] = token;
		
			token = strtok(NULL, delim);
			
		}

		//sets on of the arrays last value to NULL
	  array[j][k] = NULL;
		//if the fist command is cd
		if((strcmp(array[0][0], "cd"))==0){
				
				if(chdir (array[0][1]) == 0) {

						k=0;
						j=0;

				}else printf("%s", "CD failed");

			}
		else{
			//set the last thing in 2d array to null
			array[j+1][0] = NULL;
			
			piping(array);
			k=0;
			j=0;
			
		}

	}
	return 0;
}


			

	
  

	


