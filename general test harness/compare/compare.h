// James Jessen
// CptS 422
 
#ifndef __COMPARE_H__
#define __COMPARE_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"

// Given a list of files terminated by a null element
// Compares the files using a sophisticated algorithm
// Returns the name of the file containing the comparison
char* compare_files(char* file_list[]);

// Compares 2 lines
// Judging significance is beyond the scope of this function
// Returns number of significant differences
int line_compare(char* line_ptr1, char* line_ptr2);

// Returns true if line is significant
// Otherwise, returns false
int get_line_type(char* line_ptr);

#endif
