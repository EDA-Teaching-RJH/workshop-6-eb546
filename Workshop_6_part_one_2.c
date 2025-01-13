#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Function to create a file and add content
void create_file(const char *file_name, const char *content, const char *file_type) {
    FILE *file;

    if (strcmp(file_type, ".txt") == 0 || strcmp(file_type, ".csv") == 0) {
        file = fopen(file_name, "w");
    } else if (strcmp(file_type, ".bin") == 0) {
        file = fopen(file_name, "wb");
    } else if (strcmp(file_type, ".json") == 0) {
        file = fopen(file_name, "w");
    } else {
        printf("Unsupported file type.\n");
        return;
    }

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if (strcmp(file_type, ".bin") == 0) {
        fwrite(content, sizeof(char), strlen(content), file);
    } else {
        fprintf(file, "%s", content);
    }

    fclose(file);
    printf("File created: %s\n", file_name);
}

// Function to append content to an existing file
void append_to_file(const char *file_name, const char *content) {
    FILE *file = fopen(file_name, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
    printf("Content appended to: %s\n", file_name);
}

// Function to copy/duplicate a file
void copy_file(const char *source_file, const char *destination_file) {
    FILE *source = fopen(source_file, "rb");
    if (source == NULL) {
        perror("Error opening source file");
        return;
    }

    FILE *destination = fopen(destination_file, "wb");
    if (destination == NULL) {
        perror("Error opening destination file");
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, destination);
    }

    fclose(source);
    fclose(destination);
    printf("File copied: %s to %s\n", source_file, destination_file);
}

// Function to delete a file
void delete_file(const char *file_name) {
    if (remove(file_name) == 0) {
        printf("File deleted: %s\n", file_name);
    } else {
        perror("Error deleting file");
    }
}

// Function to read file data
void read_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    printf("\nFile read complete: %s\n", file_name);
}

int main() {
    // Example usage
    // Create a text file
    create_file("sample.txt", "Hello, this is a sample text file.\n", ".txt");

    // Append content to the text file
    append_to_file("sample.txt", "This is an appended line.\n");

    // Create a binary file
    create_file("sample.bin", "This is a binary file.", ".bin");

    // Create a CSV file
    create_file("data.csv", "Name,Age,Location\nAlice,30,New York\nBob,25,Los Angeles\n", ".csv");

    // Create a JSON file
    create_file("data.json", "{\"name\":\"Alice\", \"age\":30, \"location\":\"New York\"}\n", ".json");

    // Copy the text file
    copy_file("sample.txt", "sample_copy.txt");

    // Read content from the text file
    printf("Reading sample.txt:\n");
    read_file("sample.txt");

    // Delete the copied text file
    delete_file("sample_copy.txt");

    return 0;
}
