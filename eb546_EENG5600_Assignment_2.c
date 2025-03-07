//Include all the library headers for input, output, and string functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define the maximum length of strings and have the existing file for saving data
#define MAX_STRING_LENGTH 100
#define FILENAME "Catalogue.txt"

//Define a struct function for holding details of a song set by the user
typedef struct {
    char BandName[MAX_STRING_LENGTH];
    char SongName[MAX_STRING_LENGTH];
    float Price;
    int Year;
} Song;

//Void functions with specified arguements for setting them as a menu choice
void readData(Song catalogue[], int *count); //This function reads song data from the user
void printCatalogue(Song catalogue[], int count); //This function prints the saved catalogue to the screen
void saveDataToFile(Song catalogue[], int count); //This function saves catalogue data to a file
void loadDataFromFile(Song catalogue[], int *count); //This function loads the catalogue data from a file
void sortCatalogueByYear(Song catalogue[], int count); //This function sorts the catalogue data by year

int main() {
    Song catalogue[100]; //Storing up to 100 songs
    int count = 0;  //Keeps track to how many songs are in the catalogue
    int choice; //Declare a variable to store the user's menu choice

    
    loadDataFromFile(catalogue, &count); //Load exisiting data from the exisiting file "Catalogue.txt"

    //Using the while loop function that if its all true, it will proceed to to display the menu options
    while (1) {
        printf("---------------------------------------------------");
        printf("\nThe following options are available:\n");
        printf("Read in data [1]\n");
        printf("Print out catalogue to screen [2]\n");
        printf("Save data to file [3]\n");
        printf("Exit Program [4]\n");
        printf("Enter your choice now: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf

        //The switch function allows to store the void functions in a correct choice option.
        switch (choice) {
            case 1:
                readData(catalogue, &count);
                break;
            case 2:
                sortCatalogueByYear(catalogue, count);
                printCatalogue(catalogue, count);
                break;
            case 3:
                saveDataToFile(catalogue, count);
                break;
            case 4:
                printf("\nProgram Shutting Down...\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

//This function reads song data from the user
void readData(Song catalogue[], int *count) {
    if (*count >= 100) { //Check if the number of songs reach the maximum limit
        printf("Catalogue is full. Cannot add more songs.\n");
        return;
    }

    printf("\nEnter Band/Singer Name: "); //Ask the user to enter the name of a band or singer
    fgets(catalogue[*count].BandName, MAX_STRING_LENGTH, stdin); //When it reads the strings, it also reads the white spaces from the input to avoid errors
    catalogue[*count].BandName[strcspn(catalogue[*count].BandName, "\n")] = '\0'; //Removes a newline character with a null terminator

    printf("Enter Song Name: "); //Ask the user to enter the song's name from that band or singer
    fgets(catalogue[*count].SongName, MAX_STRING_LENGTH, stdin); //When it reads the strings, it also reads the white spaces from the input to avoid errors
    catalogue[*count].SongName[strcspn(catalogue[*count].SongName, "\n")] = '\0'; //Removes a newline character with a null terminator

    printf("Enter Price (Pounds): "); //Ask the user to enter the price of that song record in pounds
    scanf("%f", &catalogue[*count].Price); //Enter a decimal number 

    printf("Enter Year of Release: "); //Ask the user to enter the year when this song was released
    scanf("%d", &catalogue[*count].Year); //Type as a whole number
    getchar(); //Reads one character(s) at a time to have the years in order from newer to older

    (*count)++; //Increment the songs count every time the user enters a new song data to the catalogue
    printf("\nSong added successfully!\n"); //Prints out this message once all the data has been filled
}

//This function prints the saved catalogue to the screen
void printCatalogue(Song catalogue[], int count) {
    if (count == 0) { // If there aren't songs in the catalogue
        printf("Catalogue is empty.\n"); //Prints out a statement saying the catalogue is empty
        return;
    }

    printf("--------------------------------------------------------------");
    printf("\nCatalogue (Sorted by Year of Release - Newer to Older):\n");
    for (int i = 0; i < count; i++) { //The for loop prints the the details depending the number of songs entered to the catalogue file
        printf("\nName of Band/Singer: %s\n", catalogue[i].BandName);
        printf("Name of Song: %s\n", catalogue[i].SongName);
        printf("Price: £%.2f\n", catalogue[i].Price);
        printf("Year of Release: %d\n\n", catalogue[i].Year);
    }
}

//This function saves catalogue data to a file
void saveDataToFile(Song catalogue[], int count) {
    
    //Has a FILE pointer to open files by "w" or write access mode
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) { //If there is no file in the memory address
        printf("\nError opening file for writing!\n"); //Prints out a message regarding an empty data storage
        return;
    }

    //The for loop saves adn writes each data to the existing filename
    for (int i = 0; i < count; i++) {
        fprintf(file, "\n%s\n", catalogue[i].BandName);
        fprintf(file, "%s\n", catalogue[i].SongName);
        fprintf(file, "£%.2f\n", catalogue[i].Price);
        fprintf(file, "%d\n", catalogue[i].Year);
    }

    fclose(file); //Closes the file
    printf("\nData saved to file successfully!\n"); //Prints out the statement saying that the data has been saved to the file
}

//This function loads the catalogue data from a file
void loadDataFromFile(Song catalogue[], int *count) {
    
    //Has a FILE pointer to open files by "r" or read access mode
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) { //If there is no file in the memory address
        printf("\nNo existing catalogue file found! Starting with an empty catalogue.\n"); //Prints out a message regarding an empty data storage
        return;
    }

    *count = 0; //Resets the counter everytime the catalogue menu program restarts

    //Reads each song's details from the files
    while (fgets(catalogue[*count].BandName, MAX_STRING_LENGTH, file)) { //When it reads the strings, it also reads the white spaces from the input to avoid errors
        catalogue[*count].BandName[strcspn(catalogue[*count].BandName, "\n")] = '\0'; //Removes a newline character with a null terminator

        fgets(catalogue[*count].SongName, MAX_STRING_LENGTH, file); //When it reads the strings, it also reads the white spaces from the input to avoid errors
        catalogue[*count].SongName[strcspn(catalogue[*count].SongName, "\n")] = '\0'; //Removes a newline character with a null terminator

        fscanf(file, "%f\n", &catalogue[*count].Price);
        fscanf(file, "%d\n", &catalogue[*count].Year);
        (*count)++;
    }

    fclose(file); //Closes the file
    printf("\nData loaded from file successfully!\n"); //Prints out the statement saying that the data has been loaded from the file
}

//This function sorts the catalogue data by year from newer to older
void sortCatalogueByYear(Song catalogue[], int count) {
    
    //Using for loop inner functions for bubble sort to sort the songs by year in descending order
    for (int i = 0; i < count - 1; i++) { //Checks through the songs's details 
        for (int j = 0; j < count - i - 1; j++) { //Checks only once
            if (catalogue[j].Year < catalogue[j + 1].Year) { //Compares the years for each details
                
                //Swap the songs by the number of years from newest to oldest
                Song Temp = catalogue[j];
                catalogue[j] = catalogue[j + 1];
                catalogue[j + 1] = Temp;
            }
        }
    }
}