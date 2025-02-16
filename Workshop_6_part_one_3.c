#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 200
#define MAX_PATH_LENGTH 1030
#define MAX_CONTENT_LENGTH 10000

char currentDirectory[MAX_PATH_LENGTH];

void createFile(const char *filename, const char *content) {
    char filepath[MAX_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", currentDirectory, filename);

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error creating file!\n");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
    printf("File '%s' created successfully.\n", filename);
}

void appendToFile(const char *filename, const char *content) {
    char filepath[MAX_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", currentDirectory, filename);

    FILE *file = fopen(filepath, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
    printf("Content appended to '%s' successfully.\n", filename);
}

void copyFile(const char *sourceFilename, const char *destFilename) {
    char sourcePath[MAX_PATH_LENGTH], destPath[MAX_PATH_LENGTH];
    snprintf(sourcePath, sizeof(sourcePath), "%s/%s", currentDirectory, sourceFilename);
    snprintf(destPath, sizeof(destPath), "%s/%s", currentDirectory, destFilename);

    FILE *sourceFile = fopen(sourcePath, "r");
    FILE *destFile = fopen(destPath, "w");

    if (sourceFile == NULL || destFile == NULL) {
        printf("Error opening files!\n");
        return;
    }

    char ch;
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);
    printf("File '%s' copied to '%s' successfully.\n", sourceFilename, destFilename);
}

void deleteFile(const char *filename) {
    char filepath[MAX_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", currentDirectory, filename);

    if (remove(filepath) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error deleting file!\n");
    }
}

void readFile(const char *filename) {
    char filepath[MAX_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", currentDirectory, filename);

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
    printf("\nFile '%s' read successfully.\n", filename);
}

void listDirectory() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(currentDirectory);
    if (dir == NULL) {
        printf("Error opening directory!\n");
        return;
    }

    printf("Contents of '%s':\n", currentDirectory);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void changeDirectory(const char *dirname) {
    char newPath[MAX_PATH_LENGTH];
    snprintf(newPath, sizeof(newPath), "%s/%s", currentDirectory, dirname);

    if (chdir(newPath) == 0) {
        getcwd(currentDirectory, sizeof(currentDirectory));
        printf("Changed directory to '%s'\n", currentDirectory);
    } else {
        printf("Error changing directory!\n");
    }
}

void createDirectory(const char *dirname) {
    char dirpath[MAX_PATH_LENGTH];
    snprintf(dirpath, sizeof(dirpath), "%s/%s", currentDirectory, dirname);

    if (mkdir(dirpath, 0777) == 0) {
        printf("Directory '%s' created successfully.\n", dirname);
    } else {
        printf("Error creating directory!\n");
    }
}

int main() {
    int choice;
    char filename[MAX_FILENAME_LENGTH];
    char destFilename[MAX_FILENAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char dirname[MAX_FILENAME_LENGTH];

    // Initialize current directory
    getcwd(currentDirectory, sizeof(currentDirectory));

    while (1) {
        printf("\nFile Management System - Current Directory: %s\n", currentDirectory);
        printf("1. Create File\n");
        printf("2. Append to File\n");
        printf("3. Copy File\n");
        printf("4. Delete File\n");
        printf("5. Read File\n");
        printf("6. List Directory Contents\n");
        printf("7. Change Directory\n");
        printf("8. Create Directory\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename (e.g., file.txt, file.bin, file.csv, file.json): ");
                scanf("%s", filename);
                printf("Enter content: ");
                getchar(); // To consume the newline character left by scanf
                fgets(content, MAX_CONTENT_LENGTH, stdin);
                createFile(filename, content);
                break;

            case 2:
                printf("Enter filename: ");
                scanf("%s", filename);
                printf("Enter content to append: ");
                getchar(); // To consume the newline character left by scanf
                fgets(content, MAX_CONTENT_LENGTH, stdin);
                appendToFile(filename, content);
                break;

            case 3:
                printf("Enter source filename: ");
                scanf("%s", filename);
                printf("Enter destination filename: ");
                scanf("%s", destFilename);
                copyFile(filename, destFilename);
                break;

            case 4:
                printf("Enter filename to delete: ");
                scanf("%s", filename);
                deleteFile(filename);
                break;

            case 5:
                printf("Enter filename to read: ");
                scanf("%s", filename);
                readFile(filename);
                break;

            case 6:
                listDirectory();
                break;

            case 7:
                printf("Enter directory name (use '..' to go up): ");
                scanf("%s", dirname);
                changeDirectory(dirname);
                break;

            case 8:
                printf("Enter new directory name: ");
                scanf("%s", dirname);
                createDirectory(dirname);
                break;

            case 9:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

