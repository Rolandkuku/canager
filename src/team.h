//
// Created by Roland Kullet on 03/05/16.
//

#ifndef CANAGER_TEAM_H
#define CANAGER_TEAM_H

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
        // TODO: Créer un nouveau fichier avec nommé d'après le nom de la team
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
#endif //CANAGER_TEAM_H
