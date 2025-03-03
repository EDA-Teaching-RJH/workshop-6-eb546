#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100
#define FILENAME "catalogue.txt"

// Struct to hold song details
typedef struct {
    char bandName[MAX_STRING_LENGTH];
    char songName[MAX_STRING_LENGTH];
    float price;
    int year;
} Song;

// Function prototypes
void readData(Song catalogue[], int *count);
void printCatalogue(Song catalogue[], int count);
void saveDataToFile(Song catalogue[], int count);
void loadDataFromFile(Song catalogue[], int *count);
void sortCatalogueByYear(Song catalogue[], int count);

int main() {
    Song catalogue[100]; // Array to store songs
    int count = 0;       // Number of songs in the catalogue
    int choice;

    // Load existing data from file (if any)
    loadDataFromFile(catalogue, &count);

    while (1) {
        // Display menu
        printf("\nThe following options are available:\n");
        printf("Read in data [1]\n");
        printf("Print out catalogue to screen [2]\n");
        printf("Save data to file [3]\n");
        printf("Exit Program [4]\n");
        printf("Enter your choice now: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf

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
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Function to read song data from the user
void readData(Song catalogue[], int *count) {
    if (*count >= 100) {
        printf("Catalogue is full. Cannot add more songs.\n");
        return;
    }

    printf("Enter Band/Singer Name: ");
    fgets(catalogue[*count].bandName, MAX_STRING_LENGTH, stdin);
    catalogue[*count].bandName[strcspn(catalogue[*count].bandName, "\n")] = '\0'; // Remove newline

    printf("Enter Song Name: ");
    fgets(catalogue[*count].songName, MAX_STRING_LENGTH, stdin);
    catalogue[*count].songName[strcspn(catalogue[*count].songName, "\n")] = '\0'; // Remove newline

    printf("Enter Price: ");
    scanf("%f", &catalogue[*count].price);

    printf("Enter Year of Release: ");
    scanf("%d", &catalogue[*count].year);
    getchar(); // Consume newline character left by scanf

    (*count)++;
    printf("Song added successfully!\n");
}

// Function to print the catalogue to the screen
void printCatalogue(Song catalogue[], int count) {
    if (count == 0) {
        printf("Catalogue is empty.\n");
        return;
    }

    printf("\nCatalogue (Sorted by Year of Release - Newer to Older):\n");
    for (int i = 0; i < count; i++) {
        printf("Name of Band/Singer: %s\n", catalogue[i].bandName);
        printf("Name of Song: %s\n", catalogue[i].songName);
        printf("Price: %.2f\n", catalogue[i].price);
        printf("Year of Release: %d\n\n", catalogue[i].year);
    }
}

// Function to save catalogue data to a file
void saveDataToFile(Song catalogue[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", catalogue[i].bandName);
        fprintf(file, "%s\n", catalogue[i].songName);
        fprintf(file, "%.2f\n", catalogue[i].price);
        fprintf(file, "%d\n", catalogue[i].year);
    }

    fclose(file);
    printf("Data saved to file successfully!\n");
}

// Function to load catalogue data from a file
void loadDataFromFile(Song catalogue[], int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing catalogue file found. Starting with an empty catalogue.\n");
        return;
    }

    *count = 0;
    while (fgets(catalogue[*count].bandName, MAX_STRING_LENGTH, file)) {
        catalogue[*count].bandName[strcspn(catalogue[*count].bandName, "\n")] = '\0'; // Remove newline

        fgets(catalogue[*count].songName, MAX_STRING_LENGTH, file);
        catalogue[*count].songName[strcspn(catalogue[*count].songName, "\n")] = '\0'; // Remove newline

        fscanf(file, "%f\n", &catalogue[*count].price);
        fscanf(file, "%d\n", &catalogue[*count].year);
        (*count)++;
    }

    fclose(file);
    printf("Data loaded from file successfully!\n");
}

// Function to sort the catalogue by year (newer to older)
void sortCatalogueByYear(Song catalogue[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (catalogue[j].year < catalogue[j + 1].year) {
                // Swap songs
                Song temp = catalogue[j];
                catalogue[j] = catalogue[j + 1];
                catalogue[j + 1] = temp;
            }
        }
    }
}