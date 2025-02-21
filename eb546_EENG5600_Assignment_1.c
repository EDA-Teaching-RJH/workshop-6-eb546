#include <stdio.h>
#define MAX 100

struct cg 
{ // structure to hold x and y coordinates and mass
    float x, y, mass;
}masses[MAX];
    
int readin(void)
{
    /* Write this function to read in the data
    into the array masses
    note that this function should return the number of
    masses read in from the file */


}

void computecg(int n_masses)
{
   
    /* Write this function to compute the C of G
    and print the result */

    float x, y, mass;

    printf("Enter a value for x-coordinate: ");
    scanf("%s", x);

    printf("Enter a value for y-coordinate: ");
    scanf("%s", y);

    printf("Sum of x is: ");

}

int main(void)
{
    int number;
    if((number = readin()) > 0)
    computecg(number);
    return 0;
}