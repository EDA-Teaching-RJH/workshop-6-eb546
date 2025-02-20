
#include <stdio.h> //Includes the standard input/output header file
#include <stdlib.h> //Includes the standard library header file for declaring programming tasks and functions
#include <string.h> //Includes functions mainly for manipulating strings and memory blocks
#include <dirent.h> //Includes functions for working with directory operations
#include <sys/stat.h> //Includes functions for acquiring file metadata informations
#include <unistd.h> //Includes operating system API functions, like low level operations
#include <errno.h> //Includes error reporting functions in the system
#include <stdint.h> //Includes integer types with specific widths
#include <arpa/inet.h> //Includes functions for IP address and netwoek manipulations
#include <netinet/in.h> //Includes defining internet addresses and sockets
#include <sys/socket.h> //Includes functions for working with sockets

#define Max_Username_Length 50 //Defines the maximum number of characters' size for username
#define Max_Password_Langth 50 //Defines the maximum number of characters' size for password
#define Max_Filename_Length 100 //Defines the maximum number of characters' size for filename
#define Max_Path_Length 1050 //Defines the maximum number of characters' size for file path
#define Max_Content_Length 1000 //Defines the maximum number of characters' size for content
#define CRC32_Polynomial 0xEDB88320L //Defines the polynomial for CRC32 checker for error detections
#define Server_IP "127.0.0.1" //Defines the IP address of the server for networking
#define Server_Port 8080 //Defines the port number by the server within the machine itself
#define Buffer_Size 1050 //Defines the maximum number of the buffer's size 

char CurrentDirectory[Max_Path_Length]; //Defines the current directory name with a maxmimum number of length for file path

//This void function creates a new file with a specified file type and contents to be written inside
void CreateFile(const char *filename, const char *content)
{
    //Almost all void functions have this inner function with similar format to execute file system
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    //Has a FILE pointer to open files by "w" or write access mode
    FILE *file = fopen(FilePath, "w");
    if (file == NULL) {
        perror("Error creating file!");
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
    printf("\nFile '%s' was created successfully. \n", filename);

}

//This void function allows to append or add contents into the existing file(s) in the current directory
void AppendFile(const char *filename, const char *content)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    //Has a FILE pointer to open files by "a" or append access mode
    FILE *file = fopen(FilePath, "a");
    if (file == NULL)
    {
        perror("Error opening file!");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
    printf("\nContent appended to '%s' was successful.\n", filename);
}

//This void function allows to copy the contents from one file to paste to another
void CopyFile(const char *sourcefilename, const char *destfilename)
{
    char SourcePath[Max_Path_Length], DestPath[Max_Path_Length];
    snprintf(SourcePath, sizeof(SourcePath), "%s/%s", CurrentDirectory, sourcefilename);
    snprintf(DestPath, sizeof(DestPath), "%s/%s", CurrentDirectory, destfilename);

    //Has a FILE pointer to open files by "r" or read access mode
    FILE *sourcefile = fopen(SourcePath, "r");
    if (sourcefile == NULL)
    {
        perror("Error opening source file!");
        return;
    }

    //Has a FILE pointer to open files by "w" or write access mode
    FILE *destfile = fopen(DestPath, "w");
    if (destfile == NULL)
    {
        perror("Error opening destination file!");
        fclose(sourcefile);
        return;
    }

    char CH;
    while ((CH = fgetc(sourcefile)) != EOF)
    {
        fputc(CH, destfile);
    }
    fclose(sourcefile);
    fclose(destfile);
    printf("\nFile '%s' copied to '%s' was successful.\n", sourcefilename, destfilename);

}

//This function allows to read the contents from any files within the current directory
void ReadFile(const char *filename)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    //Has a FILE pointer to open files by "r" or read access mode
    FILE *file =fopen(FilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file!");
        return;
    }

    char CH;
    while((CH = fgetc(file)) != EOF)
    {
        putchar(CH);
    }
    fclose(file);
    printf("\nFile '%s' was read successfully.\n", filename);
}

//This function allows to delete any existing files in the current directory
void DeleteFile(const char *filename)
{
    char FilePath[Max_Path_Length];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", CurrentDirectory, filename);

    if (remove(FilePath) == 0)
    {
        printf("\nFile '%s' was deleted successfully.\n", filename);

    } else
    {
        perror("Error deleting file!");
    }
}


//This function allows to lists all the files contained within the directory's explorer
void ListContents()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(CurrentDirectory);
    if (dir == NULL)
    {
        perror("Error opening directory!");
        return;
    }
    printf("\nFile Contents of '%s':\n", CurrentDirectory);
    while ((entry = readdir(dir)) != NULL)
    {
        printf("\n|%s|\n", entry->d_name);
    }
    closedir(dir);
}

//This function allow to create a new directory of codespace
void CreateDirectory(const char *dirname)
{
    char DirPath[Max_Path_Length];
    snprintf(DirPath, sizeof(DirPath), "%s/%s", CurrentDirectory, dirname);

    if(mkdir(DirPath, 0777) == 0)
    {   
        printf("\nDirectory '%s' was created successfully.\n", dirname);
    } else
    {
        perror("Error creating directory!\n");
    }
}

//This function allows to change the name of the current directory of codespace
void ChangeDirectory(const char *dirname) //If you want to navigate towards the parent directory, enter "..". To go down, enter the exact same words to avoid an error.
{                                         //E.g, Enter a directory name to change: .. | Enter a directory name to change: workshop-6-eb546
    char NewPath[Max_Path_Length];
    snprintf(NewPath, sizeof(NewPath), "%s/%s", CurrentDirectory, dirname);

    if(chdir(NewPath) == 0)
    {
        getcwd(CurrentDirectory, sizeof(CurrentDirectory));
        printf("\nChanged directory to '%s'\n", CurrentDirectory);
    } else
    {
        perror("Error changing directory!\n");
    }
}

//This function adds the CRC32 checker to compare files of their validity
uint32_t calculate_crc32(const char* filename) {
    
    //Has a FILE pointer to open files by "rb" or binary read access mode
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for CRC32 calculation!");
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

//This function combines with CRC32 checker in order to completely verify the entire system operations
int compare_files_crc(const char* file1, const char* file2) {
    uint32_t crc1 = calculate_crc32(file1);
    uint32_t crc2 = calculate_crc32(file2);

    if (crc1 == 0 || crc2 == 0) {
        printf("Error calculating CRC32 checksum!\n");
        return 0;
    }

    return (crc1 == crc2);
}

//This function allows the client to send files to the server for networking
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

    //Has a FILE pointer to open files by "rb" or binary read access mode
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
    printf("File '%s' was sent successfully.\n", filename);
}

//This function allows for the server to receive files from the client for networking
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

    printf("Waiting for connections with client....\n");

    NewSockFD = accept(SockFD, (struct sockaddr *)&ClientAddr, &AddrLen);
    if(NewSockFD < 0)
    {
        perror("Accept failed!");
        close(SockFD);
        return;
    }

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        perror("Error creating file!");
        close(NewSockFD);
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
    printf("File '%s' was received succesfully.\n", filename);
}

//This function allows to start the client process to transfer files to the server's IP address and port by the void SendFile function
void StartClient(const char *serverip, int port)
{
    while(1)
    {
        printf("\nClient is ready to send files to server....\n");
        char FileName[Max_Filename_Length];
        printf("Enter filename to send (or 'exit' to quit): ");
        scanf("%s", FileName);

        if(strcmp(FileName, "exit") == 0)
        {
            printf("Client shutting down....\n");
            break;
        }

        SendFile(FileName, serverip, port);
    }
}

//This function allows to start the server process to receive the file by the void ReceiveFile function
void StartServer(int port)
{
    while(1)
    {
        printf("\nServer is waiting for files from client....\n");
        char FileName[Max_Filename_Length];
        printf("Enter filename to receive and save (or 'exit' to quit): ");
        scanf("%s", FileName);

        if(strcmp(FileName, "exit") == 0)
        {
            printf("Server shutting down....\n");
            break;
        }

        ReceiveFile(FileName, port);
    }
}

//This function builds the user authentication for a login ID before accessing the file management system
int Authenticator()
{
    char Username[Max_Username_Length];
    char Password[Max_Password_Langth];
    char FileUsername[Max_Username_Length];
    char FilePassword[Max_Password_Langth];
    FILE *file;

    printf("Enter your username: ");
    scanf("%s", Username);

    printf("Enter your password: ");
    scanf("%s", Password);

    //This function allows to read the words from the sample.txt file that contains the username and password in different ways
    file = fopen("sample.txt", "r"); //e.g (username: eron, password: eb546), (username:ERON, password: EB546), (username: ERON, password: eb546)
    if (file == NULL)                // (username: Eron, password: eb546), (username: eron, password EB546), (username: Eron, password: EB546)
    {
        perror("Error opening user file!");
        return 0;
    }

    while (fscanf(file, "%s %s", FileUsername, FilePassword) != EOF)
    {
        if (strcmp(Username, FileUsername) == 0 && strcmp(Password, FilePassword) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

//The main function stores all the structures of the functions, a starting point for executing the program
int main()
{
    if (!Authenticator())
    {
        printf("User Authentication failed. Incorrect username or password. Exiting Program.\n");
        return 1;
    }
    printf("Authentication Granted!\n");

    int Choice;
    char FileName[Max_Filename_Length];
    char DestFileName[Max_Filename_Length];
    char Content[Max_Content_Length];
    char DirName[Max_Filename_Length];

    getcwd(CurrentDirectory, sizeof(CurrentDirectory));

    while(1)
    {   
        printf("----------------------------------------------------------------------------------");
        printf("\nFile Management System Program - Current Directory: %s\n", CurrentDirectory);

        printf("\n1. Create File\n");
        printf("2. Append File\n");
        printf("3. Copy File\n");
        printf("4. Read File\n");
        printf("5. Delete File\n");
        printf("6. List of Explorer Contents\n");
        printf("7. Create New Directory\n");
        printf("8. Change Current Directory Name\n");
        printf("9. Compare Files (CRC32 Checker)\n");
        printf("10. Start Client (Send Files)\n");
        printf("11. Start Server (Receive Files)\n");
        printf("12. Exit\n");
        printf("----------------------------------------------------------------------------------");
        printf("\nEnter a number (1-12): ");
        scanf("%d", &Choice);

        //This function allows the user to choose one of the choice numbers only from 1 to 12
        switch (Choice)
        {
            case 1:
                printf("Enter file type to create (file.txt, file.bin, file.csv, file.json, file.c, etc): ");
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
                printf("Enter a filename to read: ");
                scanf("%s", FileName);
                ReadFile(FileName);
                break;

            case 5:
                printf("Enter a filename to delete: ");
                scanf("%s", FileName);
                DeleteFile(FileName);
                break;

            case 6:
                ListContents();
                break;

            case 7:
                printf("Enter a new directory name in explorer: ");
                scanf("%s", DirName);
                CreateDirectory(DirName);
                break;

            case 8:
                printf("Enter a directory name to navigate down('..' to navigate up): ");
                scanf("%s", DirName);
                ChangeDirectory(DirName);
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
                StartClient(Server_IP, Server_Port);
                break;

            case 11:
                StartServer(Server_Port);
                break;

            case 12:
                printf("Shutting Down Program\n");
                exit(0);

            default:
                printf("Invalid number or choice! Please try again.\n");

        }

    }

    return 0;
}
