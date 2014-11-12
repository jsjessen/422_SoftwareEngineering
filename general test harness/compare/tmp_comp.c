#include <stdio.h>
#include "compare.h"

int main()
{
    char* file_list[] = { "fileA", "fileB", "fileC", NULL };

    char* output_file = compare_files(file_list);
    
    printf("Comparison file: %s\n", output_file);

    return 0;
}
