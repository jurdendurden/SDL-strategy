#include "main.h"

///Generates a random number within the given range.
int rnd_num(int min, int max)
{ 
    int num = 0;

    if (min == 0 && max == 0)
        return 0;    

    num = (min + rand() % (max+1 - min));

    return num;
    
}

//Prints errors to the console and logs them into game.log
void add_log(char * message)
{
    printf("%s", message);
    FILE * fp;

    if ((fp = fopen("../game.log", "a")) == NULL)
    {
        printf("Error writing to log file.");
        return;
    }
    

    fprintf(fp, "%s", message);
    fclose(fp);
}