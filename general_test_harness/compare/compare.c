#include "compare.h"

typedef enum { false, true } bool;
typedef enum { BLOCK_START, NUMBERS, OTHER } line_type;

static char* DELIMS = " -=*\t\n";


int get_line_type(char* line_ptr)
{ 
    int i = 0;
    int word_len = 0;
    int direct_len = 0;
    char** words = parse(line_ptr, DELIMS);

    if(!words || !words[0])
        return OTHER;

    // First and last work begin with a digit/number
    if(isdigit(words[0][0]) && isdigit(words[i][0]))
        return NUMBERS;

    while(words[i + 1]) 
    { 
        int j;
        for(j = 0; words[i][j]; j++)
            words[i][j] = tolower(words[i][j]);

        //printf("%s ", words[i]);
        i++; 
    }
    //putchar('\n');
    if(i < 1)
        return OTHER;

    word_len = strlen(words[i - 1]);
    direct_len = strlen("direct");

    if(i < 1 || word_len < direct_len)
        return OTHER;

    // Ends with "...direct X", as in "direct blocks", "indirect blocks", etc...
    if(strcmp(&words[i - 1][word_len - direct_len], "direct") == 0) 
        return BLOCK_START;

    return OTHER;
}


int line_compare(char* line_ptr1, char* line_ptr2)
{    
    int i = 0;
    int diff_count = 0;
    char** words1 = parse(line_ptr1, DELIMS);
    char** words2 = parse(line_ptr2, DELIMS);
    int length1 = 0;
    int length2 = 0;

    if(!words1 || !words2)
        return -1;

    if(!words1[0] && !words2[0])
        return 0;

    while(words1[i] && words2[i])
    {
        if(strcmp(words1[i], words2[i]) != 0)
            diff_count++;

        i++;
    }

    while(words1[length1++]) {}
    while(words2[length2++]) {}

    if(length1 > length2)
        diff_count += (length1 - length2);
    else
        diff_count += (length2 - length1);

    return diff_count;
}


// Acknowledges that only 2 things can be compared at a time.
//
// Uses LCS_length(X,Y)/MAX(Length(X),Length(Y)) ratio to measure similarity of X and Y
//
// Defines ancor points as matching similarities between files
// Specifically, matching frequency of a LCS_strA in fileA and LCS_strB in fileB
// They serve as a boundary when investigating line mismatch
//
// Assumes: 
// Case and and whitespace is not important
//      With the exception of newline 
//      Lines provide a useful atomic element more informative for readers than characters
// Lines nearest the current position are more likely to match
// Regions with similar LCS_length require high intolerance to differentiate similar lines
//
// The bottom of the file will have a list of lines with no match found in the other file
//
// This function focuses on micro differences rather than macro
// because macro differences are easy to spot, micro are not
char* compare_files(char* file_list[]) 
{
    // Algorithm
    // ---------
    // 1) Only compare 2 files at a time, so compare each file with every other file
    //    Be careful not to compare A to B and then B to A.
    //
    //    1) Create an outfile with a name identifiable as a combination of the 2 files being compared
    //
    //    2) Get a char* array for each of the 2 files being compared, where each element is a line
    //       The array should be null terminated
    //       Use this array for outputting lines of text so the reader sees what they expect
    //
    //    3) Copy the arrays, skipping over any characters not considered important (like whitespace)
    //       and setting all alpha characters tolower()
    //       Use this array for comparisons
    //
    //    4) Compute the strlen() for each element in the comparison arrays
    //       Keep track of this in a parallel array for each
    //
    //    5) Search for Ancor lines and keep track of their indices
    //       Think of ancor lines as a bar connecting an indice in each file
    //       When searching for line mismatch, it is useful to have a local boundary that should never be crossed alone (the bar)
    //       That is, a bar should only ever be crossed by both file's Current Line simultaniously
    //       Ancor matching should be very intolerent, a false positive could completly ruin the output
    //       Just like naive line by line comparison is ruined by a single line mismatch
    //
    //       If the Ancors found are unreliable and/or the the #Ancors/#lines ration is high
    //       Define a Reach variable
    //       Its value is how far to search when investigating a line mismatch
    //
    //       For instance, if one file always proceeds commands with "Input Command: " and the other with "CMD: "
    //       Their LCS should always have the prefix of "Input Command: " and "CMD: " respectively
    //       There should be the same frequency of occurance in both files
    //
    //       1) Create variables to keep track of the index of the Current Line for each file's array
    //          Invariant: All lines before Current Line have either been matched or found to have no match
    //
    //       2) Start with the Current Line = 0 for both
    //
    //          1) Pick the Current Line from one of the files, it doesn't matter which one
    //             strcmp it with the other file's Current Line
    //
    //                  match) Print the line to the outfile
    //                         and then increment the Current Line for both files
    //               
    //               mismatch) Calculate the LCS_length for it and 
    //                         every line from the Current to the next Ancor (or within Reach)
    //                         Store the values in a local scope array
    //                         Divide each of these values by the string length of their respective line (determined previously)
    //                         Determine the the line with the maximum ratio (betwee 0 and 1)
    //
    //                      within Tolerance) If the ratio is within Tolerance, treat the lines as a match with minor difference
    //                                        Print the line from first file, 
    //                                        the difference, 
    //                                        then the line from the second file to output
    //                                        Set the Current Line to both of these lines
    //
    //                 not within Tolderance) If the ratio is not within Tolerance, 
    //                                        append it to a list of lines without match, prefixed by filename
    //
    //            Repeat 
    //
    //
    // May need to adjust values for variables like Tolerance and Reach for different scenarios


    // isalnum(c)
    // tolower(c)

    return "comp_file";
}
//*err_line1 = 0;
//*err_line2 = 0;

// Open files for read
//if((fp1 = fopen(filename1, "r")) == NULL)
//{
//    fprintf(stderr, "Unable to open: %s\n", filename1);
//    exit(EXIT_FAILURE);
//}
//if((fp2 = fopen(filename2, "r")) == NULL)
//{
//    fprintf(stderr, "Unable to open: %s\n", filename2);
//    exit(EXIT_FAILURE);
//}

//// Get both to first block
//while(((read1 = getline(&line_ptr1, &len1, fp1)) != -1) &&
//        (get_line_type(line_ptr1) != BLOCK_START)) { line_count1++; }

//while(((read2 = getline(&line_ptr2, &len2, fp2)) != -1) &&
//        (get_line_type(line_ptr2) != BLOCK_START)) { line_count2++; }

//while(true)
//{
//    int new_diff = 0;

//    // Keep going until hit a NUMBERS line
//    while(((read1 = getline(&line_ptr1, &len1, fp1)) != -1) &&
//            (get_line_type(line_ptr1) != NUMBERS)) { line_count1++; }

//    while(((read2 = getline(&line_ptr2, &len2, fp2)) != -1) &&
//            (get_line_type(line_ptr2) != NUMBERS)) { line_count2++; }

//    // If one file is at the end, the remainder of the other is different
//    if(read1 == -1)
//    {
//        while((read2 = getline(&line_ptr2, &len2, fp2)) != -1)
//        {
//            diff_count++;
//            puts(line_ptr2);
//        }
//        break;
//    }
//    if(read2 == -1)
//    {
//        while((read1 = getline(&line_ptr1, &len1, fp1)) != -1)
//        {
//            diff_count++;
//            puts(line_ptr1);
//        }
//        break;
//    }

//    if((new_diff = line_compare(line_ptr1, line_ptr2)) > 0)
//    {
//        if(*err_line1 == 0)
//            *err_line1 = line_count1;

//        if(*err_line2 == 0)
//            *err_line2 = line_count2;

//        diff_count += new_diff;
//        printf("%s: %s", filename1, line_ptr1);
//        printf("%s: %s\n", filename2, line_ptr2);
//    }
//}

//free(line_ptr1);
//free(line_ptr2);

//fclose(fp1);
//fclose(fp2);

//return diff_count;
