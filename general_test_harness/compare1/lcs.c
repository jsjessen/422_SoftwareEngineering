// Adapted from http://www.geeksforgeeks.org/printing-longest-common-subsequence/

/* Dynamic Programming implementation of LCS problem */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


/* Utility function to get max of 2 integers */
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int LCS( char *X, char *Y, int m, int n, char** lcs_str )
{
    int i = 0;
    int j = 0;
    int index = 0;

    int L[m+1][n+1];

    /* Following steps build L[m+1][n+1] in bottom up fashion. Note
       that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
    for (i=0; i<=m; i++)
    {
        for (j=0; j<=n; j++)
        {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i-1] == Y[j-1])
                L[i][j] = L[i-1][j-1] + 1;
            else
                L[i][j] = max(L[i-1][j], L[i][j-1]);
        }
    }

    // Following code is used to print LCS
    index = L[m][n];

    // Create a character array to store the lcs string
    if((*lcs_str = (char*)malloc((index + 1) * sizeof(char))) == NULL)
    {
        perror("LCS: malloc");
        return 0;
    } 
    (*lcs_str)[index] = '\0'; // Set the terminating character

    // Start from the right-most-bottom-most corner and
    // one by one store characters in lcs_str[]
    i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X[] and Y are same, then
        // current character is part of LCS
        if (X[i-1] == Y[j-1])
        {
            (*lcs_str)[index-1] = X[i-1]; // Put current character in result
            i--; j--; index--;     // reduce values of i, j and index
            continue;
        }

        // If not same, then find the larger of two and
        // go in the direction of larger value
        if (L[i-1][j] > L[i][j-1])
            i--;
        else
            j--;
    }

    return L[m][n];
}

/* Driver program to test above function */
int main()
{
    char* X = "a x b y c z p d q";
    char* Y = "a b c a x b y c z";

    int m = strlen(X);
    int n = strlen(Y);
    char* lcs_str = NULL;
    int lcs_len = LCS(X, Y, m, n, &lcs_str);

    printf("LCS Length: %d\n", lcs_len);
    puts(lcs_str);
    free(lcs_str);

    return 0;
}

