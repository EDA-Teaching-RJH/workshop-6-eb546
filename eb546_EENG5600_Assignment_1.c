#include <stdio.h> //Includes the standard input/output library heading
#define MAX 100 //Defines the constant MAX to represent the macimum number of heights


struct cg 
{ // structure to hold x and y coordinates and mass
    float x, y, mass; // Members of the structures as decimal numbers
}masses[MAX]; //Declare an array of structures named masses


int readin(void)
{
    /* Write this function to read in the data
    into the array masses
    note that this function should return the number of
    masses read in from the file */

    int n_masses; //Declare a variable for the user to enter the number of weights as an integer
    printf("Enter the number of weights: "); //Prompt the user to ener the number of weights
    scanf("%d", &n_masses); //To read the number of weights from the user to store it in n_masses

    if (n_masses > MAX) //If the number of weights exceeds the maximum number
        {
            printf("Error! Number of wights exceeded the maximum limit.\n"); //Prints out an error handling message
            return 0; //Return 0 to indicate an error
        }

    for (int i = 0; i < n_masses; i++) //Loop to read the data for each weight depending the user's number
        {
            printf("Enter data of weight for each coordinates %d (x_coordinate, y_coordinate, mass): ", i +1); //Prompt the user to enter the data for each coordinates
            scanf("%f %f %f", &masses[i].x, &masses[i].y, &masses[i].mass); //Reads the data of weight for each coordinates to store it in an array
        }

    return n_masses; //Returns a value if the number was read successfuly
}

void computecg(int n_masses)
{
   
    /* Write this function to compute the C of G
    and print the result */

    float sum_of_xmass = 0.0; //Declare variables to store the sum of xmass as a float
    float sum_of_ymass = 0.0; //Declare variables to store the sum of ymass as a float
    float sum_of_mass = 0.0; //Declare variables to store the sum of mass as a float

    for(int i = 0; i < n_masses; i++) //Using the for loop to calculate the required sums
        {
            sum_of_xmass += masses[i].x * masses[i].mass; //Add the sum of xmass and x-coordinate * mass together
            sum_of_ymass += masses[i].y * masses[i].mass;  //Add the sum of xmass and y-coordinate * mass together
            sum_of_mass += masses[i].mass;  //Add the sum of xmass and mass together
        }

    if (sum_of_mass == 0) //Set an error handling if the total mass is 0
        {
            printf("Error! Result is 0. Cannot compute center of gravity!.\n"); //Prints out an error statement if the system detects a 0
            return; //Returns a value to exit the function
        }
    
    float cg_x = sum_of_xmass / sum_of_mass; //Calculate the center of gravity of the x-coordinate as a decimal number
    float cg_y = sum_of_ymass / sum_of_mass; //Calculate the center of gravity of the y-coordiante as a decial number

    printf("COG coordinates are: x = %.2f y = %.2f\n", cg_x, cg_y); //Print out the result of the center of gravity coordinates moving one decimal place
    
}

int main(void) //the mai function
{
    int number; //Declare a variable to store the number of weights returned by readin()
    if((number = readin()) > 0) //Using the if function to call int read() to read the data and check if it returns a valid result for weights
    computecg(number); //Calling void computecg() to proceed its function to calculate and print the center of gravity
    return 0; //Return the 0 value as a success execution for the program
}