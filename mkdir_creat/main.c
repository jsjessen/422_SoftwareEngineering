#include <stdio.h>

#include <global.h>
#include <input.h>
#include <print.h>
#include <memory.h>
#include <cmd.h>

#include <init.c>

typedef struct state 
{
    int parent_ino;
    int my_ino;

    int  rec_len;
    int  name_len;
    char name[256];

    u16 type;
    u16 links_count;
    int size;
    u16 uid;
    u16 gid;

} STATE;

// Gloabl Input and Expected Arrays
char** Input  = NULL;
struct STATE* Expect[32];

void fill_expected(struct STATE* newState, char* line) {
	char* token = NULL, part = NULL; copyLine = NULL;
	char* tokens[10];
	int i = 0;
	
	// Copy the line because we will destroy it
	copyLine = (char *) malloc(strlen(line) + 1 *sizeof(char));
	strcpy(copyLine, line);
	
	//tokenize input before putting into expected
	token = strtok(line, " ");
	tokens[i++] = token;
	while (token = strtok(NULL, " "))
	{
		tokens[i++] = token;
	}
	i=0;
	// Populate the expected state
	newState->parent_ino = strtol(tokens[i++], &part, 10);
	newState->my_ino = strtol(tokens[i++], &part, 10);
	newState->rec_len = strtol(tokens[i++], &part, 10);
	newState->name_len = strtol(tokens[i++], &part, 10);
	strcpy(newState->name, tokens[i++]);
	newState->type = strtol(tokens[i++], &part, 10);
	newState->links_count = strtol(tokens[i++], &part, 10);
	newState->size = strtol(tokens[i++], &part, 10);
	newState->uid = strtol(tokens[i++], &part, 10);
	newState->gid = strtol(tokens[i++], &part, 10);
}

int line_check(char* line) {
	char* commands[3] = { "mkdir", "creat", "cd"};
	char* subStr = NULL;
	int i;
	
	// Check for blank line
	if (strlen(line) < 2)
		return 0;
	
	// Check if the line is a comment
	if (line[0] == '#')
		return 0;
	
	// Check if line has a command in it
	for (i = 0; i < 3; i++)
	{
		if ( subStr = strstr(line, commands[i]) ) //contains one of the command substrings
			return 1;
	}
	// If we are here, then the line must be an expected line
	return 2;
}
	

	
void populate_arrays(char* filename) // Eventually need to free all of the allocated memory after all test commands are performed
{
	FILE* testfile = NULL;
	ssize_t read;
	ssize_t len = 256;
	
	char path[64];
	char* line;
	int line_check = 0, i = 0, j = 0;
	bool isPair = false;
	
	struct state* temp_Expect = NULL;
	char* temp_command = NULL;
	
	
	// Create the path for the test file
	strcat(path, "test/");
	strcat(path, filename);
	
	testfile = fopen(path, "r");
	if (!testfile)
	{
		printf("Error opening file: %s\n", path);
		return;
	}
	
	// Start reading
	while ( (read = getline(&line, &len, testfile)) != -1)
	{
		line_check = check_line(line);
		// Input line
		if (line_check == 1) 
		{
			//fill Input with newly allocated string
			temp_command = (char *) malloc(strlen(line) + 1 * sizeof(char));
			strcpy(temp_command, line);
			//printf("temp_command: %s\n", temp_command);
			Input[i++] = temp_command;
		 
		}
		// Expect Line
		if (line_check == 2)
		{
			// fill Expected with newly allocated Expect
			temp_Expect = (STATE *) malloc(sizeof(STATE));
			fill_expected(temp_Expect, line);
			Expect[j++] = temp_Expect;
			
		}
	}
}


void quit_test()
{
    int i = 0;
    MINODE* mip = NULL;

    for(i = 0; i < NMINODES; i++)
    {
        mip = &MemoryInodeTable[i];

        if(mip->refCount > 0)
            iput(mip);
    }
    close(running->cwd->device);
}

bool isStateMatch(int argc, char* argv[], int device, STATE expected)
{
    printf("Command: '%s'  Parameter: '%s'\n", argv[0], argv[1]);

    bool error = false;

    char* path = argv[1];
    char* dirname  = NULL;
    char* basename = NULL;
    parse_path(path, &dirname, &basename);

    int parent_ino = getino(device, dirname);
    int my_ino     = getino(device, path);

    INODE* parent_ip = get_inode(device, parent_ino);
    INODE* my_ip     = get_inode(device, my_ino);

    char name[1024]; // Should only need 256
    int rec_len = 0;
    int name_len = 0;

    u8* block = get_block(device, my_ip->i_block[0]);
    DIR* dp = (DIR*)block;

    rec_len = dp->rec_len;
    name_len = dp->name_len;
    strncpy(name, dp->name, name_len);
    name[name_len] = '\0';

    // Parent Inode
    if(parent_ino != expected.parent_ino)
    {
        error = true; 
        fprintf(stderr, "Parent Inode Number: %d"
                "\tExpected: %d\n", parent_ino, expected.parent_ino); 
    }

    // My Inode
    if(my_ino != expected.my_ino)
    {
        error = true; 
        fprintf(stderr, "My Inode Number: %d"
                "\tExpected: %d\n", my_ino, expected.my_ino); 
    }

    //RETHINK
    if(strlen(basename) != expected.name_len)
    {
        error = true; 
        fprintf(stderr, "Name Length: %d"
                "\tExpected: %d\n", strlen(basename), expected.name_len); 
    }

    // Record Length
    if(rec_len != expected.rec_len)
    {
        error = true; 
        fprintf(stderr, "Record Length: %d"
                "\tExpected: %d\n", rec_len, expected.rec_len); 
    }
    // Name Length
    if(name_len != expected.name_len)
    {
        error = true; 
        fprintf(stderr, "Name Length: %d"
                "\tExpected: %d\n", name_len, expected.name_len); 
    }

    // Name
    if(strcmp(basename, expected.name) != 0)
    {
        error = true; 
        fprintf(stderr, "Name: %s"
                "\tExpected: %d\n", basename, expected.my_ino); 
    }

    // Type
    if((my_ip->i_mode & 0xF000) != expected.type)
    {
        error = true; 
        fprintf(stderr, "Mode: %#o"
                "\tExpected: %#o\n", my_ip->i_mode, expected.my_ino); 
    }

    // Links Count
    if(my_ip->i_links_count != expected.links_count)
    {
        error = true; 
        fprintf(stderr, "Links Count: %d"
                "\tExpected: %d\n", my_ip->i_links_count, expected.links_count); 
    }

    // Size
    if(my_ip->i_size != expected.size)
    {
        error = true; 
        fprintf(stderr, "Size: %d"
                "\tExpected: %d\n", my_ip->i_size, expected.size); 
    }

    // User ID
    if(my_ip->i_uid != expected.uid)
    {
        error = true; 
        fprintf(stderr, "UID: %d"
                "\tExpected: %d\n", my_ip->i_uid, expected.uid); 
    }

    // Group ID
    if(my_ip->i_gid != expected.gid)
    {
        error = true; 
        fprintf(stderr, "GID: %d"
                "\tExpected: %d\n", my_ip->i_gid, expected.gid); 
    }

    free(dirname);
    free(basename);
    free(parent_ip);
    free(my_ip);

    if(error)
        return false;
    else
        return true;
}

int main(int argc, char* argv[])
{
    // For each test file in directory test:
    {
        char* device_name = NULL;
        int device = 0, j = 0;
		char* test_file_names[3] = {  "test0", "test1", NULL };
		

        // get name of device
        device_name = argv[1];


        initialize_fs(); 
        mount_root(device_name);

        running = &ProcessTable[0];
        running->status = READY;
        running->cwd = root; root->refCount++;
        running->uid = SUPER_USER;
        
        // Populate Arrays
        populate_arrays(test_file_names[j]);
        // Loop entire program for each test file?


        // Loops through all elements of Input
        int i = 0;
        while(true)
        {
            bool isMatch = false;

            char** cmd_argv = NULL;
            int    cmd_argc = 0;
            int  (*cmd_fptr)(int, char**) = NULL; 

            if(!Input[i])
            {
                quit_test();
                break;
            }

            cmd_argv = parse(Input[i], " "); // Parse input into cmd argv[]

            while(cmd_argv[++cmd_argc]){}    // Determine cmd argc

            cmd_fptr = get_cmd(cmd_argv[0]); // Get the command's function pointer
            cmd_fptr(cmd_argc, cmd_argv);    // Execute the command with parameters

            // Check if current state matches expected state
            // If they don't match, stop testing for this test file 
            // because the state mismatch will likely cascade after this point 
            isMatch = isStateMatch(cmd_argc, cmd_argv, device, Expect[i]);
            free_array(cmd_argv);

            if(!isMatch)
                break;

            i++;
        }
    }

    return 0;
}
