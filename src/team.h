//
// Created by Roland Kullet on 03/05/16.
//

#ifndef CANAGER_TEAM_H
#define CANAGER_TEAM_H

void makeNewTeam();
int makeNewTeammate();


#include <unistd.h>
#include <dirent.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <memory.h>
#include "functions.h"

void printTeammate(xmlNodePtr pNode);
void listTeammates(xmlDocPtr doc);
void parseTeammate(xmlDocPtr doc, xmlNodePtr cur, char *teammate);

// ****************** Managing XML FILES *************** //

void saveXmlDoc(xmlDocPtr doc, char *doc_name) {
    if(doc != NULL) {
        xmlSaveFormatFile(doc_name, doc, 1);
    } else {
        fprintf(stderr, "Unknown file");
    }
    return;
}

void createTeamFile(char *filename, char *file_path) {
    int rc;
    xmlTextWriterPtr writer;
    xmlDocPtr doc;
    xmlChar *tmp;


    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("makeNewTeam: Error creating the xml writer\n");
        return;
    }

    // encoding
    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0) {
        printf("makeNewTeam: Error at xmlTextWriterStartDocument\n");
        return;
    }
    // start root element
    rc = xmlTextWriterStartElement(writer, BAD_CAST "team");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }
    // set team name
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST filename);
    if (rc < 0) {
        printf("xmlTextWriterWriteAttribute: error while setting team name");
    }
    // ned root element
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }
    xmlFreeTextWriter(writer);
    xmlSaveFileEnc(file_path, doc, "UTF-8");
    xmlFreeDoc(doc);
}

// ****************** PARSING TEAMMATES ****************** //

void xmlParseTeammate(xmlDocPtr doc, char *doc_name, Teammate *teammate) {
    xmlNodePtr cur;
    xmlNodePtr new_node;
    xmlAttrPtr new_attr;
    cur = xmlDocGetRootElement(doc);
    new_node = xmlNewTextChild(cur, NULL, "teammate", NULL);
    // last name
    new_attr = xmlNewProp(new_node, "last_name", (const xmlChar *)teammate->last_name);
    new_attr = xmlNewProp(new_node, "first_name", (const xmlChar *)teammate->first_name);
}

void listTeammates(xmlDocPtr doc) {
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"teammate"))) {
            printTeammate(cur);
        }
        cur = cur->next;
    }
}

void printTeammate(xmlNodePtr cur) {
    xmlChar *data;
    data = xmlGetProp(cur, (const xmlChar *)"first_name");
    printf("First Name : %s  ", data);
    data = xmlGetProp(cur, (const xmlChar *)"last_name");
    printf("Last Name : %s \n", data);
    xmlFree(data);
    return;
}

void parseTeammate(xmlDocPtr doc, xmlNodePtr cur, char *teammate) {
    xmlNewTextChild(cur, NULL, "teammate", teammate);
    return;
}

void makeTeammate(Teammate *teammate) {
    // First name
    printf("/ /********** New Teammate *********/ /\n");
    printf("First name :    \n");
    scanf("%s", teammate->first_name);
    // Last name
    printf("Last name :    \n");
    scanf("%s", teammate->last_name);
    // Email
    printf("Email :   \n");
    scanf("%s", teammate->email);
    // Skill
    int skill;
    printf("Which skill ? \n 1. BACK \n 2. FRONT \n 3. SELLS \n");
    scanf("%d", &skill);
    switch (skill) {
        case 1 :
            teammate->skill = BACK;
            break;
        case 2 :
            teammate->skill = FRONT;
            break;
        case 3 :
            teammate->skill = SELLS;
        default:
            // TODO return err;
            break;
    }
}

//***************** END PARSING TEAMMATES ********* //

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

    printf("Teammates already registered : \n\n");
    listTeammates(doc);

    Teammate teammate;
    makeTeammate(&teammate);
    xmlParseTeammate(doc, file_path, &teammate);
    saveXmlDoc(doc, file_path);

    printf("New teammate list : \n");
    listTeammates(doc);
    xmlFreeDoc(doc);
}


void makeNewTeam() {
    char *filename = malloc(sizeof(char)*50);
    char *file_path;
    printf("Name of the team : \n");
    scanf("%s", filename);
    file_path = strcat(getcwd(0, 0), "/data/");
    file_path = strcat(strcat(file_path, filename), ".xml");

    createTeamFile(filename, file_path);
}
#endif //CANAGER_TEAM_H
