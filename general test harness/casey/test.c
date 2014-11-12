#include <stdio.h>                                                
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>                    
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h> 

#include "compare.h"

#define FILE_PERM 0644
#define MAX_NUM_GETCHARS 100


int run_program(char* program, char* command, char* disk, char *outfileNames[], int count)
{
    int i = 0;
    int pd[2];
    int pid = 0;
    int status = 0;
    char outfile[128];
    char path[128];
  	int read = 0;  
    FILE* file = NULL;
    char* line;
    ssize_t len;
    
    
    //create local filepath
   	/*getcwd(path, 128);
   	strcat(path, "/");
   	strcat(path command);*/
    
    //create outfile name from current run
	strcpy(outfile, disk); 
	strcat(outfile, ".out");
	
    // Create Pipe
    if(pipe(pd) != 0)
        perror("run_program(): creating pipe");

    // Fork child process
    if((pid = fork()) < 0)
        perror("run_program(): forking child process");

    if(pid) //Reads command file and outputs the data to the pipe
    {       // Parent Writer

        close(pd[0]);// writer MUST close PipeIn 
        close(1);    // close stdout 
        dup(pd[1]);  // replace stdout with PipeOut 
		
		if ((file = fopen(command, "r")) == NULL ) {
			printf("run_program: Error opening %s\n", command);
			exit(0);
		}
		// Read the command File
		while ( (read = getline(&line, &len, file)) != -1) {
			line[len-1] = 0; //get rid of \n character
			//send commands to program
			printf("%s", line);
		}
		fclose(file);
		return pid;
		//execlp(command, command, (char *) NULL); //Execute the command shell script    

    }
    else
    {       // Child Reader        

        close(pd[1]); // reader MUST close PipeOut
        close(0);     // close stdin
        dup(pd[0]);   // replace stdin with PipeIn

        // Redirect stdout to outfile
        close(1);
        if(open(outfile, O_WRONLY | O_CREAT, FILE_PERM) < 0)
            perror("open outfile");
            
        // Insert outfile name to list
		//outfileNames[count++] = outfile;
		
        fprintf(stderr, "Writing to %s\n", outfile);
		// Run ext 2 file system program
        execlp(program, program, disk, (char*)NULL);
        fprintf(stderr, "Execution of %s FAILED\n", program);
        exit(1);
    } 
}

int copyImage (char* disk, char* filename) {

	int pid;
	int status;
	
	if ( (pid = fork()) < 0)
		perror("copyFile: Forking for copy failed");
	if (pid)
	{
		waitpid(pid, &status, 0);
		return 1;
	}
	else
	{
		execl("/bin/cp", "/bin/cp", disk, filename, NULL); //run cp on the image, create copy in cwd
		exit(1);
		return -1;
		
	}
}
	
char* outfileNames[6];
int out_count = 0;

int main()
{
    char* disk = "fdimage";
    int program_count = 2;
    char* program[] = { "j_program", "c_program", "kc_program", NULL };
    char* Disk[] = { "disk_0", "disk_1", "disk_2", "disk_3", "disk_4", NULL };
    char* Command[] = { "cmd_0", "cmd_1", "cmd_2", NULL };

    int i = 0, j = 0, k = 0; //for simplicity sake, using once disk right now, can easily be changed later
    while( program[i] ) {
    	while ( Command[j] ) {
    		int pid;
    		int status;
    		int pd[2];
    		char copyFile[128];
    		
    		// Create filename for this iteration
    		strcpy(copyFile, program[i]); // Add program name
    		strcat(copyFile, "_");
    		strcat(copyFile, Command[j]); // Add command script filename
    		strcat(copyFile, "_");
    		strcat(copyFile, Disk[0]); // Add disk name
    		printf("Copyfile name: %s\n", copyFile);
    		
    		
    		printf("\n--------- New Iteration --------\n\n");
    		
    		if ( (pid = fork())  < 0 )
    			perror("main: Forking child process failed\n");
    			
    		if (pid) //forking to a child process to copy file
    		{
    			waitpid(pid, &status, 0);
    		}
    		else //execute copying of image file  
    		{
    			int child;
    			
    			/*if ( copyImage(Disk[0], copyFile) < 0 )
    				printf("Copyfile failed on %s\n", copyFile);*/
    			printf("\n Running %s with %s as input.\n", program[i], Command[i]);
    			// run the program after file copy
    			child = run_program(program[i], Command[j], copyFile, outfileNames, out_count);	 
    			
    			waitpid(child, &status, 0);
    			exit(0);
    		}
    		printf("\n--------- End Iteration --------\n\n");
    		j++;
    	}
    	i++;
    }
    outfileNames[out_count] = NULL;
    printf("out_count = %d\n", out_count);
    //compare(outfileNames);		
  			

    printf("\n----------------------------------------------\n\n");
    return 0;
}
