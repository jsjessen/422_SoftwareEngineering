// Casey O'Leary
// CS 360 Main for ext 2 file system
#include "type.h"
#include "lab8422.h"


// Takes an input string, which it destroys
// splits on a whitespace to two already allocated pointers
void split(char *input, char *head, char *tail) 
{
		char *token = strtok(input, " ");
		strcpy(head, token);
		//printf("command=%s\n", head);
		token = strtok(NULL, " ");
		if (token != NULL)
		{
			strcpy(tail, token);
			//printf("path=%s\n", tail);
		}
		return;
}

int run_command(char *cmd, char *path)
{
	int j;
	for (j = 0; j < 11; j++) 
	{
		//printf("Comparing: %s to %s\n");
		if (strcmp(cmnds[j], cmd) == 0)
		{
			printf("Executing: %s %s\n", cmd, path);
			return fptr[j](path);
		}
	}
}

//int expected(char *cmd, char *path)

// Function that frees up any allocated memory during testing period for the two arrays of pointers
void clean_memory(char **commands, char **pathnames)
{
	int i = 0;
	while (commands[i] != NULL)
	{
		free(commands[i++]);
		//memset(commands[i++], 0, sizeof(commands[i++]));
	}
	i = 0;
	while (pathnames[i] != NULL)
	{
		free(pathnames[i++]);
		//memset(pathnames[i++], 0, sizeof(pathnames[i++]));
	}
	return;
}


int command_type(char *command) 
{
	
	if (strcmp(command, "mkdir") == 0)
		return 3;
	else if (strcmp(command, "creat") == 0)
		return 2;
	else if (strcmp(command, "cd") == 0)
		return 1;
	else
		return 0;
}

int exists(char *path)
{
	int num;
	num = getino(dev, path);
	if ( num > 0 )
	{
		return num;
	}
	else
		return num;
}

void check_cmd(int *flag, int return_value, int *error_count, char *path)
{
	int inode_number = exists(path);
	// First check if command returned positive value
	if (return_value > 0) // Succesful return value
	{
		// Check fail flag, was this command supposed to be succesful or not?
		if (*(flag) > 0)
		{
			printf("Invalid: Command returned successful, but was not supposed too.\n");
			*(error_count) = *(error_count) + 1;
		}
		else // Command was supposed to be succussful
		{
			// Make sure path has a valid inode number
			if (inode_number > 0)
			{
				printf("Valid result: Command executed succesfully. INODE#: %d\n", inode_number);
			}
			else
			{
				printf("Invalid result: succesful return value, but invalid INODE#\n");
				*(error_count) = *(error_count) + 1;
			}
		}
		*(flag) = 0;
		
	}
	else // Command returned an error, check if it was supposed to fail or not
	{
		
		if (*(flag) > 0)
		{
			printf("Vaid result: Command failed as it should.\n");
		}
		else
		{
			printf("Invalid result: Command failed when it should not have.\n");
			*(error_count) = *(error_count) + 1;
		}
		*(flag) = 0;
	}
}


	

int main(int argc, char *argv[])
{
	int i, j, k;
	int cmd = 0, error_count = 0, execution_value = 0, fail_flag = 0, pid = 0, status = 0, type = -1, total = 0;
	unsigned int ino_num = -1;
	
	//input files
	char *files[4] = { "input0", "input1", "input2", NULL };
	
	ssize_t read, len = 512;
	FILE *input = NULL;
	
	/*// arrays for the commands, assuming there are less than 25 commands/pathnames and they will not be longer
	// than 32 characters
	char *commands[32];
	char *pathnames[32];*/
	
	char *line = NULL, *command = NULL, *path = NULL;
	
	// Initialization of ext2 file system
	init();
	mount_root(argv[1]);
	printf("Dev=%d, inodeBegin=%d, bmap=%d, imap=%d, ninodes=%d\n", dev, inodeBegin, bmap, imap, ninodes);
	printf("ext_2 file system initialized....\n\n");
	
	k = 0;
	// Loop through disks and input files
	while (files[k] != NULL)
	{
		// Start 422 mkdir_creat testing
		if (!(input = fopen(files[k], "r"))) // check for error opening file from command args, this is the input file
		{
			//printf("Error opening file %s\n",files[k]);
			break;
		}
		printf("\n*********************** Starting mkdir & creat testing ***********************\n");
		// Get input from file
		i = 0;
		line = (char *) malloc(len +1);
		while ((read = getline(&line, &len, input)) != -1) 
		{
			line[strlen(line)-1] = 0; // kill \r at end
		
			// Check for the fail flag, meaning the command is supposed to fail based on previous inputs
			if (line[0] != '#')
			{
				//allocate space for each new command and path
				command = (char *) malloc(128+1);
				path = (char *) malloc(512+1);
				
				// split up the input
				split(line, command, path);
		
				// put into arrays and store for later in case we want to do something else with them, i dont know what yet
				// had ideas initially, but then changed approach, still could do something with this later
				/*commands[i] = command; 
				pathnames[i] = path;*/
				type = command_type(command);
				
				// Run the Command and set the error checking value, 
				// which is already returned in the function so no need to duplicate code
				execution_value = run_command(command, path);
				total++;
				
	
				switch(type)
				{
					case 3: check_cmd(&fail_flag, execution_value, &error_count, path);
						break;
					case 2: check_cmd(&fail_flag, execution_value, &error_count, path);
						break;
					case 1: printf("\n**CD command executed, not yet considered in this test\n");
						break;
					case 0: printf("\nInvalid command input for this automated test.\n");
						break;
					default: printf("\n");
				}
				i++;
			}
			else // # character in file indicates command should fail, or have an error during execution
				fail_flag = 1;
			
			printf("\n");
		}
		free(command);
		free(path);
		fclose(input);
		k++;
		
		// Fork to a different process to clean the disk
		if ( (pid = fork()) < 0)
		{
			printf("Error forking.\n");
			exit(0);
		}
		if (pid)
		{
			// Do nothing until child is done
			//waitpid(pid, &status, 0);
		}
		else
		{
			char *program = "clean.sh";
			close(1); //close stdout so we dont see all the junk
			execl(program, program, argv[0], NULL);
			exit(0);
		}
			
	}
	
	printf("\n------------------------------ End Error Results ----------------------------- \n");
	if (error_count == 0)
	{
		printf("\n SUCCESS! All commands executed correctly and ones that expected to fail did!\n");
	}
	else
	{
		printf(" Almost! There were %d errors out of %d total commands.\n", error_count, total);
	}
	return;
}
