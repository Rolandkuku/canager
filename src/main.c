/*
    Created by Renaud Bellec
    
    Main file of Canager project
*/

#include <stdio.h>
#include "task.h"

int main () {
    char toto[sizeof(char)*100];
    Task task;
    task.name = "My task";
    printf("%s\n", task.name);
}