#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define Max_Filename_Length 100
#define Max_Path_Length 1024
#define Max_Content_Length 1000
#define CRC32_Polynomial 0xEDB88320L
#define Server_IP "127.0.0.1"
#define Server_Port 8080
#define Buffer_Size 1024

char CurrentDirectory[Max_Path_Length];


void CreateFile(const char *filename, const char *content)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    FILE *file = fopen(FilePath, "w");
    if (file == NULL) {
        perror("Error creating file!");
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
    printf("File '%s' created successfully. \n", filename);

}

void AppendFile(const char *filename, const char *content)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    FILE *file = fopen(FilePath, "a");
    if (file == NULL)
    {
        perror("Error opening file!");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
    printf("Content appended to '%s' successfully.\n", filename);
}

void CopyFile(const char *sourcefilename, const char *destfilename)
{
    char SourcePath[Max_Path_Length], DestPath[Max_Path_Length];
    snprintf(SourcePath, sizeof(SourcePath), "%s/%s", CurrentDirectory, sourcefilename);
    snprintf(DestPath, sizeof(DestPath), "%s/%s", CurrentDirectory, destfilename);

    FILE *sourcefile = fopen(SourcePath, "r");
    if (sourcefile == NULL)
    {
        perror("Error opening source file!");
        return;
    }

    FILE *destfile = fopen(DestPath, "w");
    if (destfile == NULL)
    {
        perror("Error opening destination file!");
        fclose(sourcefile);
        return;
    }

    char ch;
    while ((ch = fgetc(sourcefile)) != EOF)
    {
        fputc(ch, destfile);
    }
    fclose(sourcefile);
    fclose(destfile);
    printf("File '%s' copied to '%s' successfully.\n", sourcefilename, destfilename);

}

void DeleteFile(const char *filename)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    if (remove(FilePath) == 0)
    {
        printf("File '%s' deleted successfully.\n", filename);

    } else
    {
        perror("Error deleting file!");
    }
}

void ReadFile(const char *filename)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    FILE *file =fopen(FilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file!");
        return;
    }

    char ch;
    while((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }
    fclose(file);
    printf("File '%s' read successfully.\n", filename);
}

void ListDirectory()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(CurrentDirectory);
    if (dir == NULL)
    {
        perror("Error opening directory!");
        return;
    }
    printf("Contents of '%s':\n", CurrentDirectory);
    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void ChangeDirectory(const char *dirname)
{
    char NewPath[Max_Path_Length];
    snprintf(NewPath, sizeof(NewPath), "%s/%s", CurrentDirectory, dirname);

    if(chdir(NewPath) == 0)
    {
        getcwd(CurrentDirectory, sizeof(CurrentDirectory));
        printf("Changed directory to '%s'\n", CurrentDirectory);
    } else
    {
        perror("Error changing directory!");
    }
}

void CreateDirectory(const char *dirname)
{
    char DirPath[Max_Path_Length];
    snprintf(DirPath, sizeof(DirPath), "%s/%s", CurrentDirectory, dirname);

    if(mkdir(DirPath, 0777) == 0)
    {
        printf("Directory '%s' created successfully.\n", dirname);
    }else
    {
        perror("Error creating directory!");
    }
}

uint32_t calculate_crc32(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for CRC32 calculation");
        return 0;
    }

    uint32_t crc = 0xFFFFFFFF;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        crc ^= ch;
        for (int i = 0; i < 8; i++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_Polynomial;
            } else {
                crc = crc >> 1;
            }
        }
    }
    fclose(file);
    return crc;
}

int compare_files_crc(const char* file1, const char* file2) {
    uint32_t crc1 = calculate_crc32(file1);
    uint32_t crc2 = calculate_crc32(file2);

    if (crc1 == 0 || crc2 == 0) {
        printf("Error calculating CRC32 checksum.\n");
        return 0;
    }

    return (crc1 == crc2);
}

void SendFile(const char *filename, const char *serverip, int port)
{
    int SockFD;
    struct sockaddr_in ServerAddr;
    FILE *file;
    char Buffer[Buffer_Size];
    ssize_t BytesRead, BytesSent;

    SockFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if (SockFD < 0)
    {
        perror("Error creating socket!");
        return;
    }

    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverip, &ServerAddr.sin_addr) <= 0)
    {
        perror("Invalid address no supported!");
        close(SockFD);
        return;
    }

    if (connect(SockFD, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        perror("Connection failed!");
        close(SockFD);
        return;
    }
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file!");
        close(SockFD);
        return;
    }
    while((BytesRead = fread(Buffer, 1, Buffer_Size, file)) > 0)
    {
        BytesSent = send(SockFD, Buffer, BytesRead, 0);
        if (BytesSent < 0)
        {
            perror("Error sending file!");
            break;
        }
    }
    fclose(file);
    close(SockFD);
    printf("File '%s' send successfully.\n", filename);
}

void ReceiveFile(const char *filename, int port)
{
    int SockFD, NewSockFD;
    struct sockaddr_in ServerAddr, ClientAddr;
    socklen_t AddrLen = sizeof(ClientAddr);
    FILE *file;
    char Buffer[Buffer_Size];
    ssize_t BytesReceived;

    SockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SockFD < 0)
    {
        perror("Error creating socket!");
        return;
    }

    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(SockFD, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        perror("Binding failed!");
        close(SockFD);
        return;
    }

    if(listen(SockFD, 1) < 0)
    {
        perror("Listen failed!");
        close(SockFD);
        return;
    }

    printf("Waiting for incoming connections...\n");

    NewSockFD = accept(SockFD, (struct sockaddr *)&ClientAddr, &AddrLen);
    if(NewSockFD < 0)
    {
        perror("Accept failed!");
        close(SockFD);
        return;
    }

    while((BytesReceived = recv(NewSockFD, Buffer, Buffer_Size, 0)) > 0)
    {
        fwrite(Buffer, 1, BytesReceived, file);
    }

    fclose(file);
    close(NewSockFD);
    close(SockFD);
    printf("File '%s' received succesfully.\n", filename);
}

void StartClient(const char *serverip, int port)
{
    while(1)
    {
        printf("\nClient is ready to send files....\n");
        char FileName[Max_Filename_Length];
        printf("Enter filename to send (or 'Exit' to quit): ");
        scanf("%s", FileName);

        if(strcmp(FileName, "Exit") == 0)
        {
            printf("Client shutting down....\n");
            break;
        }

        SendFile(FileName, serverip, port);
    }
}

void StartServer(int port)
{
    while(1)
    {
        printf("\nServer is waiting for files....\n");
        char FileName[Max_Filename_Length];
        printf("Enter filename to receive and save (or 'Exit' to quit): ");
        scanf("%s", FileName);

        if(strcmp(FileName, "Exit") == 0)
        {
            printf("Server shutting down....\n");
            break;
        }

        ReceiveFile(FileName, port);
    }
}

int main()
{
    int Choice;
    char FileName[Max_Filename_Length];
    char DestFileName[Max_Filename_Length];
    char Content[Max_Content_Length];
    char DirName[Max_Filename_Length];

    getcwd(CurrentDirectory, sizeof(CurrentDirectory));

    while(1)
    {
        printf("\nFile Management System - Current Directory: %s\n", CurrentDirectory);

        printf("1. Create File\n");
        printf("2. Append File\n");
        printf("3. Copy File\n");
        printf("4. Delete File\n");
        printf("5. Read FIle\n");
        printf("6. List of Directories\n");
        printf("7. Change Directory\n");
        printf("8. Create Directory\n");
        printf("9. Compare Files (CRC32 Checker)\n");
        printf("10. Start Client (Send Files)\n");
        printf("11. Start Server (Receive Files)\n");
        printf("12. Exit\n");
        printf("Enter a number or choice: ");
        scanf("%d", &Choice);

        switch (Choice)
        {
            case 1:
            printf("Enter file type to create (e.g., file.txt, file.bin, file.csv, file.json, file.c): ");
            scanf("%s", FileName);
            printf("Enter content(s) inside: ");
            getchar();
            fgets(Content, Max_Content_Length, stdin);
            CreateFile(FileName, Content);
            break;

            case 2:
            printf("Enter the name of the file: ");
            scanf("%s", FileName);
            printf("Enter content(s) to append inside: ");
            getchar();
            fgets(Content, Max_Content_Length, stdin);
            AppendFile(FileName, Content);
            break;

            case 3:
            printf("Enter a source filename to copy: ");
            scanf("%s", FileName);
            printf("Enter destination filename to paste: ");
            scanf("%s", DestFileName);
            CopyFile(FileName, DestFileName);
            break;

            case 4:
            printf("Enter a filename to delete: ");
            scanf("%s", FileName);
            DeleteFile(FileName);
            break;

            case 5:
            printf("Enter a filename to read: ");
            scanf("%s", FileName);
            ReadFile(FileName);
            break;

            case 6:
            ListDirectory();
            break;

            case 7:
            printf("Enter a directory name to change: ");
            scanf("%s", DirName);
            ChangeDirectory(DirName);
            break;

            case 8:
            printf("Enter a new directory name to create: ");
            scanf("%s", DirName);
            CreateDirectory(DirName);
            break;

            case 9:
            printf("Enter the first filename: ");
            scanf("%s", FileName);
            printf("Enter the second filename to compare: ");
            scanf("%s", DestFileName);
            
            if(compare_files_crc(FileName, DestFileName))
            {
                printf("Files are identical.\n");
            } 
            else
            {
                printf("Files are different.\n");
            }
            break;

            case 10:
            StartServer(Server_Port);
            break;

            case 11:
            StartClient(Server_IP, Server_Port);
            break;

            case 12:
            printf("Exiting Program\n");
            exit(0);

            default:
            printf("Invalid number or choice! Please try again.\n");

        }

    }

    return 0;
}
