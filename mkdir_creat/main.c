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
    
    int name_len;
    char[256] name;

    u16 mode;
    u16 links_count;
    int size;
    u16 uid;
    u16 gid;

} STATE;

int test_quit()
{
    int i = 0;
    MINODE* mip = NULL;
    
    for(i = 0; i < NMINODES; i++)
    {
        mip = &MemoryInodeTable[i];

        if(mip->refCount > 0)
            iput(mip);
    }
}

bool isStateMatch(int cmd_argc, int char* cmd_argv[], char* device, STATE Expect[i])
{
    // Using stat to verify
    
    return true;
}

int main(int argc, char* argv[])
{
    initialize_fs(); 
    mount_root(device);

    running = &ProcessTable[0];
    running->status = READY;
    running->cwd = root; root->refCount++;
    running->uid = SUPER_USER;


    // For each test file in directory test:
    {
        static char* device = NULL;

        // Parallel arrays
        static char** Input  = NULL;
        static STATE* Expect = NULL;


        // get name of device
        // populate Input[]
        // populate Expect[]


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
                test_quit();
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
