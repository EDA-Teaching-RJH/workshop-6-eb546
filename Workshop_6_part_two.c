#include <stdio.h>

int main() 
{
    FILE *fp;
    char ch;
    int charCount = 0;

    // Open the file in read mode
    fp = fopen("example.txt", "r");

    // Check if the file was opened successfully
    if (fp == NULL) 
    {
        printf("Error opening file!\n");
        return 1;
    }

    // Read the file character by character
    while ((ch = fgetc(fp)) != EOF) 
    {
        printf("%c", ch);
        charCount++;
    }

    // Close the file
    fclose(fp);
    printf("\nTotal number of characters: %d\n", charCount);
    
    return 0;
}