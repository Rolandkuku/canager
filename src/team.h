//
// Created by Roland Kullet on 03/05/16.
//

#ifndef CANAGER_TEAM_H
#define CANAGER_TEAM_H

int makeNewTeam();

#include <unistd.h>
#include <dirent.h>

int makingTeam() {
    printf("\n\n\n-- Team Menu --\n");
    printf("1 - New Team\n");
    printf("2 - New Teammate\n");
    printf("2 - List Teammates\n");
    printf("3 - Return to main title\n");
    char *choice = malloc(sizeof(char)*2);
    readInput(choice, 2);
    if (strncmp(choice, "1", 1) == 0) {
        // Créer une team
        // TODO: Vérifier l'existence d'un fichier avant de créer
        makeNewTeam();
    }
    else if (strncmp(choice, "2", 1) == 0) {
        // Créer un teammate
        // TODO: choisir parmis les fichier team celui de l'on veut puis y ajouter un teammate avec ses compétences
    }
    else if (strncmp(choice, "3", 1) == 0) {
        // Lister les teammates
        // TODO: choisir parmis les fichiers team et afficher le contenu
    }
    else if (strncmp(choice, "4", 1) == 0) {
        return 1;
    }
}

int makeNewTeam() {
    char * cwd;
    cwd = strcat(getcwd(0, 0), "/data/");
    printf("Name of the team : \n");
    char * file_name = malloc(sizeof(char)*50);
    fgets(file_name, sizeof(file_name), stdin);
    clean(file_name, stdin);
    file_name = strcat(file_name, ".xml");
    // Look for other filenames
    DIR * dp;
    struct dirent * ep;
    dp = opendir(cwd);
    if (dp != NULL) {
        while (ep =readdir(dp)) {
            if (strcmp(file_name, ep->d_name) == 0) {
                printf("Team %s already exists !", file_name);
                return 1;
            }
        }
        closedir(dp);
    } else {
        printf("Couldn't open directory");
        return 1;
    }

    // Creating new file
    char * file_path = strcat(cwd, file_name);
    FILE* team_file = NULL;
    team_file = fopen(file_path, "w+");
    if (team_file != NULL) {
        printf("Created new file with the name : %s", file_name);
        fclose(team_file);
        return 0;
    } else {
        printf("Cannot open team file");
        return 1;
    }
}
#endif //CANAGER_TEAM_H
