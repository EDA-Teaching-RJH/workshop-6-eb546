//Include all the library headers for input, output, and string functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define the maximum length of strings and have the existing file for saving data
#define MAX_STRING_LENGTH 100
#define FILENAME "Catalogue.txt"

//Define a struct function for holding details of a song set by the user
typedef struct {
    char bandName[MAX_STRING_LENGTH];
    char songName[MAX_STRING_LENGTH];
    float price;
    int year;
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
    fgets(catalogue[*count].bandName, MAX_STRING_LENGTH, stdin); //When it reads the strings, it also reads the white spaces from the input to avoid errors
    catalogue[*count].bandName[strcspn(catalogue[*count].bandName, "\n")] = '\0'; //Avoid creating a newline 

    printf("Enter Song Name: "); //Ask the user to enter the song's name from that band or singer
    fgets(catalogue[*count].songName, MAX_STRING_LENGTH, stdin); //When it reads the strings, it also reads the white spaces from the input to avoid errors
    catalogue[*count].songName[strcspn(catalogue[*count].songName, "\n")] = '\0'; //Avoid creating a newline 

    printf("Enter Price (Pounds): "); //Ask the user to enter the price of that song record in pounds
    scanf("%f", &catalogue[*count].price); //Enter a decimal number 

    printf("Enter Year of Release: "); //Ask the user to enter the year when this song was released
    scanf("%d", &catalogue[*count].year); //Type as a whole number
    getchar(); //Reads one character(s) at a time to have the years in order from newer to older

    (*count)++; //Increment the songs count every time the user enters a new song data to the catalogue
    printf("\nSong added successfully!\n"); //Prints out this message once all the data has been filled
}

//This function prints the saved catalogue to the screen
void printCatalogue(Song catalogue[], int count) {
    if (count == 0) {
        printf("Catalogue is empty.\n");
        return;
    }

    printf("--------------------------------------------------------------");
    printf("\nCatalogue (Sorted by Year of Release - Newer to Older):\n");
    for (int i = 0; i < count; i++) {
        printf("\nName of Band/Singer: %s\n", catalogue[i].bandName);
        printf("Name of Song: %s\n", catalogue[i].songName);
        printf("Price: £%.2f\n", catalogue[i].price);
        printf("Year of Release: %d\n\n", catalogue[i].year);
    }
}

//This function saves catalogue data to a file
void saveDataToFile(Song catalogue[], int count) {
    
    //Has a FILE pointer to open files by "w" or write access mode
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("\nError opening file for writing!\n");
        return;
    }

    //The for loop saves adn writes each data to the existing filename
    for (int i = 0; i < count; i++) {
        fprintf(file, "\n%s\n", catalogue[i].bandName);
        fprintf(file, "%s\n", catalogue[i].songName);
        fprintf(file, "£%.2f\n", catalogue[i].price);
        fprintf(file, "%d\n", catalogue[i].year);
    }

    fclose(file); //Closes the file
    printf("\nData saved to file successfully!\n"); //Prints out the statement saying that the data has been saved to the file
}

//This function loads the catalogue data from a file
void loadDataFromFile(Song catalogue[], int *count) {
    
    //Has a FILE pointer to open files by "r" or read access mode
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("\nNo existing catalogue file found! Starting with an empty catalogue.\n");
        return;
    }

    *count = 0; //Resets the counter everytime the catalogue menu program restarts

    //Reads each song's details from the files
    while (fgets(catalogue[*count].bandName, MAX_STRING_LENGTH, file)) {
        catalogue[*count].bandName[strcspn(catalogue[*count].bandName, "\n")] = '\0'; // Remove newline

        fgets(catalogue[*count].songName, MAX_STRING_LENGTH, file);
        catalogue[*count].songName[strcspn(catalogue[*count].songName, "\n")] = '\0'; // Remove newline

        fscanf(file, "%f\n", &catalogue[*count].price);
        fscanf(file, "%d\n", &catalogue[*count].year);
        (*count)++;
    }

    fclose(file); //Closes the file
    printf("\nData loaded from file successfully!\n"); //Prints out the statement saying that the data has been loaded from the file
}

//This function sorts the catalogue data by year from newer to older
void sortCatalogueByYear(Song catalogue[], int count) {
    
    //Using for loop inner functions for bubble sort to sort the songs by year in descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (catalogue[j].year < catalogue[j + 1].year) {
                
                //Swap the songs if they are out of order
                Song temp = catalogue[j];
                catalogue[j] = catalogue[j + 1];
                catalogue[j + 1] = temp;
            }
        }
    }
}