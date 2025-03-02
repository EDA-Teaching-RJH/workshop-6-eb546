#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_String_Length 100
#define FILENAME "Catalogue.txt"

typedef struct
{
   char BandName[Max_String_Length]; 
   char SongName[Max_String_Length];
   float price;
   int year;

} Song;

void ReadData(Song Catalogue[], int *count);
void PrintCatalogue(Song Catalogue[], int count);
void SaveDataToFile(Song Catalogue[], int count);
void LoadDataFromFile(Song Catalogue[], int *count);
void SortCatalogueByYear(Song Catalogue[], int count);

int main()
{
   Song Catalogue[100];
   int count = 0;
   int choice;

   LoadDataFromFile(Catalogue, &count);

   while(1)
   {
      printf("---------------------------------------------------");
      printf("\nThe following options are available:\n");
      printf("Read in data [1]\n");
      printf("Print out catalogue to screen [2]\n");
      printf("Save data to file [3]\n");
      printf("Exit Program [4]\n");
      printf("Enter your choice now: ");
      scanf("%d", &choice);
      getchar();

      switch(choice)
      {
         case 1:
            ReadData(Catalogue, &count);
            break;
         case 2:
            SortCatalogueByYear(Catalogue, count);
            PrintCatalogue(Catalogue, count);
            break;
         case 3:
            SaveDataToFile(Catalogue, count);
            break;
         case 4:
            printf("\nProgram Shutting Down....\n");
            exit(0);
         default:
            printf("\nInvalid choice! Please try again!\n");
      }
   }

   return 0;
}

void ReadData(Song Catalogue[], int *count)
{
   if(*count >= 100)
   {
      printf("Catalogue is full! Cannot add more songs.\n");
      return;
   }

   printf("Enter Band/Singer Name: ");
   fgets(Catalogue[*count].BandName, Max_String_Length, stdin);
   Catalogue[*count].BandName[strcspn(Catalogue[*count].BandName, "\n")] = '\0';

   printf("Enter Song Name: ");
   fgets(Catalogue[*count].SongName, Max_String_Length, stdin);
   Catalogue[*count].SongName[strcspn(Catalogue[*count].SongName, "\n")] = '\0';

   printf("Enter Price (Pounds): ");
   scanf("%f", &Catalogue[*count].price);

   printf("Enter Year of Release: ");
   scanf("%d", &Catalogue[*count].year);
   getchar();

   (*count)++;
   printf("\nSong added successfully!\n");
}

void PrintCatalogue(Song Catalogue[], int count)
{
   if (count == 0)
   {
      printf("\nCatalogue is empty!\n");
      return;
   }

   printf("----------------------------------------------------------------");
   printf("\nCatalogue (Sorted by Year of Release - Newer to Older):\n");
   for(int i; i < count; i++)
   {
      printf("Name of Band/Singer: %s\n", Catalogue[i].BandName);
      printf("Name of Song: %s\n", Catalogue[i].SongName);
      printf("Price: £%.2f\n", Catalogue[i].price);
      printf("Year of Release: %d\n\n", Catalogue[i].year);
   }
}

void SaveDataToFile(Song Catalogue[], int count)
{
   FILE *file = fopen(FILENAME, "w");
   if (file == NULL)
   {
      printf("\nError opening file for writing!\n");
      return;
   }

   for(int i = 0; i < count; i++)
   {
      fprintf(file, "\n%s\n", Catalogue[i].BandName);
      fprintf(file, "%s\n", Catalogue[i].SongName);
      fprintf(file, "%.2f\n", Catalogue[i].price);
      fprintf(file, "%d\n", Catalogue[i].year);
   }

   fclose(file);
   printf("\nData saved to file successfully!\n");
}

void LoadDataFromFile(Song Catalogue[], int *count)
{
   FILE *file = fopen(FILENAME, "r");
   if(file == NULL)
   {
      printf("\nNo existing catalogue file found! Starting with an empty catalogue.\n");
      return;
   }

   *count = 0;
   while(fgets(Catalogue[*count].BandName, Max_String_Length, file))
   {
      Catalogue[*count].BandName[strcspn(Catalogue[*count].BandName, "\n")] = '\0';
   

      fgets(Catalogue[*count].SongName, Max_String_Length, file);
      Catalogue[*count].SongName[strcspn(Catalogue[*count].SongName, "\n")] = '\0';

      fscanf(file, "%f\n", &Catalogue[*count].price);
      fscanf(file, "%d\n", &Catalogue[*count].year);
      (*count)++;

   }

   fclose(file);
   printf("\nData loaded from file successfully!\n");
}

void SortCatalogueByYear(Song Catalogue[], int count)
{
   for(int i = 0; i < count - 1; i++)
   {
      for(int j = 0; j < count - i - 1; j++)
      {
         if(Catalogue[j].year < Catalogue[j + 1].year)
         {
            Song Temp = Catalogue[j];
            Catalogue[j] = Catalogue[j + 1];
            Catalogue[j + 1] = Temp;
         }
      }
   }
}

