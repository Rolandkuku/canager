//
// Created by Roland Kullet on 03/05/16.
//

#ifndef CANAGER_TEAM_H
#define CANAGER_TEAM_H

int makeNewTeam();
int makeNewTeammate();


#include <unistd.h>
#include <dirent.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <memory.h>
#include "functions.h"

void parseTeammate(xmlDocPtr pDoc, xmlNodePtr pNode);

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
        makeNewTeammate();
    }
    else if (strncmp(choice, "3", 1) == 0) {
        // Lister les teammates
        // TODO: choisir parmis les fichiers team et afficher le contenu
    }
    else if (strncmp(choice, "4", 1) == 0) {
        return 1;
    }
}

int makeNewTeammate() {
    int nb_files = 1;
    char * files[100];
    char * cwd;
    cwd = strcat(getcwd(0, 0), "/data/");
    DIR * dp;
    struct dirent * ep;
    dp = opendir(cwd);
    if (dp != NULL) {
        while((ep = readdir(dp))) {
            files[nb_files] = ep->d_name;
            nb_files++;
        }
    }
    for(int i = 3; i < (nb_files); i++) { // Begin at 3 because of ./ and ../
        printf("%d -> ", i-3);
        printf("%s\n", files[i]);
    }

    char *chosenTeam = malloc(sizeof(char)*1);
    printf("Select a team : \n");
    readInput(chosenTeam, 2);
    long filePosition = strtol(chosenTeam, NULL, 10);
    char *chosenFile = files[filePosition + 3];
    printf("You have chosen %s \n", files[filePosition + 3]);

    xmlDocPtr doc;
    xmlNodePtr cur;
    char  *file_path;
    file_path = strcat(getcwd(0, 0), "/data/");
    strcat(file_path, chosenFile);
    doc =  xmlParseFile(file_path);
    if (doc == NULL) {
        fprintf(stderr, "Doc not parsed successfully. \n");
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        printf("Empty team. \n");
        xmlFreeDoc(doc);
    }


    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"teammate"))) {
            parseTeammate(doc, cur);
        }
        cur = cur->next;
    }
}

void parseTeammate(xmlDocPtr doc, xmlNodePtr cur) {
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if((!xmlStrcmp(cur->name, (const char *)"name"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            printf("Name : %s\n", key);
            xmlFree(key);
        }
        cur = cur->next;
    }
    return;
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
        while ((ep = readdir(dp))) {
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
        fputs("<?xml version='1.0' encoding='UTF-8'?><team name='team_file></team>'", team_file);
        printf("Created new file with the name : %s", file_name);
        fclose(team_file);
        return 0;
    } else {
        printf("Cannot open team file");
        return 1;
    }
}
#endif //CANAGER_TEAM_H
