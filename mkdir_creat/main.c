#include <stdio.h>

#include <global.h>
#include <input.h>
#include <print.h>
#include <memory.h>
#include <cmd.h>

#include <init.c>

typedef struct expected_outcome
{
    char[256] name;
    int ino;
    int parent_ino

    int free_blocks_count;
    // Compare with:
    // s_free_blocks_count;
    // bg_free_blocks_count;

    int free_inodes_count;
    // Compare with:
    // s_free_inodes_count;
    // bg_free_inodes_count;

    // Compare with inode's corresponding value
    u16 mode;
    u16 uid;
    u16 gid;
    int size;
    u16 links_count;
    u32 blocks;
    u32 block_0;
    u32 block_1;

} Expect;

int main(int argc, char* argv[])
{
    // Loop through test0, test1, testx, until unable to open
    static const char* device = "disk/test_disk";

    // Input[] & Expect[] are parallel arrays
    // except the last "quit" entry
    //
    // Make static global
    // Populate from test_file
    static const char* Input[] =
    {   
        "mkdir apple",
        "mkdir pear",
        "cd pear",
        "quit"
    };

    // Make static global
    // Populate from test_file
    static const Expect[] = 
    {
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
    };

    initialize_fs(device); 

    running = &ProcessTable[0];
    running->status = READY;
    running->cwd = root; root->refCount++;
    running->uid = SUPER_USER;

    int i = 0;
    while(true)
    {
        char** cmd_argv = NULL;
        int    cmd_argc = 0;
        int  (*cmd_fptr)(int, char**) = NULL; 

        cmd_argv = parse(Input[i], " "); // Parse input into cmd argv[]

        while(cmd_argv[++cmd_argc]){}    // Determine cmd argc

        cmd_fptr = get_cmd(cmd_argv[0]); // Get the command's function pointer
        cmd_fptr(cmd_argc, cmd_argv);    // Execute the command with parameters
        free_array(cmd_argv);

        // Compare with Expect[i]
        // If a test fails, go to next test file
        // because the error will cascade

        //compare(cmd_argc, cmd_argv);

        i++;
    }

    return 0;
}
