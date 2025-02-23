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

    int n_masses;
    printf("Enter the number of weights: ");
    scanf("%d", &n_masses);

    if (n_masses > MAX)
        {
            printf("Error! Number of wights exceeded the maximum limit.\n");
            return 0;
        }

    for (int i = 0; i < n_masses; i++)
        {
            printf("Enter data of weight for each coordinates %d (x_coordinate, y_coordinate, mass): ", i +1);
            scanf("%f %f %f", &masses[i].x, &masses[i].y, &masses[i].mass);
        }

    return n_masses;
}

void computecg(int n_masses)
{
   
    /* Write this function to compute the C of G
    and print the result */

    float sum_of_xmass = 0.0;
    float sum_of_ymass = 0.0;
    float sum_of_mass = 0.0;

    for(int i = 0; i < n_masses; i++)
        {
            sum_of_xmass += masses[i].x * masses[i].mass;
            sum_of_ymass += masses[i].y * masses[i].mass;
            sum_of_mass += masses[i].mass;
        }

    if (sum_of_mass == 0)
        {
            printf("Error! Result is 0. Cannot compute center of gravity!.\n");
            return;
        }
    
    float cg_x = sum_of_xmass / sum_of_mass;
    float cg_y = sum_of_ymass / sum_of_mass;

    printf("COG coordinates are: x = %.2f y = %.2f\n", cg_x, cg_y);
    
}

int main(void)
{
    int number;
    if((number = readin()) > 0)
    computecg(number);
    return 0;
}