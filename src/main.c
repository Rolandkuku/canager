/*
    Created by Renaud Bellec
    
    Main file of Canager project
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "data_struct.h"
#include "functions.h"
#include "team.h"

int main () {

    void mainTitle();
    int running = 1;
    mainTitle();

    return 0;
}

// ************** FUNCTIONS ******************* //


void mainTitle() {
    char *choice = malloc(sizeof(char)*2);
    printf("-- Welcome -- \n");
    printf("1 - Make new team\n");
    printf("2 - Make new project\n");
    printf("3 - Exit\n");
    printf("Your choice : \n");
    readInput(choice, 2);

    if (strncmp(choice, "1", 1) == 0) {
        printf("Making a team !\n");
        // TODO: making team logic
        if (makingTeam() == 1) {
            mainTitle();
        }
    }
    else if (strncmp(choice, "2", 1) == 0) {
        printf("Making a project !\n");
        // TODO: making project logic
        mainTitle();
    }
    else if (strncmp(choice, "3", 1) == 0) {
        printf("Exiting");
    }
    else {
        printf("Something went wrong\n");
        mainTitle();
    }
}
