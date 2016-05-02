/*
    Created by Renaud Bellec
    
    Main file of Canager project
*/

#include <stdio.h>
#include <stdlib.h>
#include "data_struct.h"

int main () {

    int mainTitle(int *p_running);
    int running = 1;
    while (running != 0) {
        mainTitle(&running);
    }

    return 0;
}

// ************** FUNCTIONS ******************* //


int mainTitle(int *p_running) {
    printf("-- Welcome -- \n");
    printf("1 - Make new team\n");
    printf("2 - Make new project\n");
    printf("3 - Exit\n");
    char choice = "0";
    printf("Your choice : ");
    scanf(&choice);
    printf("%d", choice);

    if (choice == (int) 1) {
        printf("Making a team !\n");
        // TODO: making team logic
    }
    else if (choice == (int) 2) {
        printf("Making a project !\n");
        // TODO: making project logic
    } else if (choice == (int) 3) {
        printf("Exiting");
        *p_running = 0;
        return 0;
    } else if (choice == (int) 0){
        printf("Something went wrong\n");
    }

    return 0;
}