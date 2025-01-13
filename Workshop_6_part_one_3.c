#include <stdio.h>

int main() {
    const char *filename = "myfile.txt";
    
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error deleting file '%s'.\n", filename);
    }
    
    return 0;
}
