//
// Created by Roland Kullet on 02/05/16.
//

#ifndef CANAGER_FUNCTIONS_H
#define CANAGER_FUNCTIONS_H

#include <dirent.h>
#include <unistd.h>
#include <libxml/xmlwriter.h>

void clearBuffer() {
    int c = 0;
    while(c != '\n' && c != EOF) {
        c = getchar();
    }
}

int readInput(char *str, int len) {
    char *returnPosition = NULL;
    if(fgets(str, len, stdin) != NULL) {
        returnPosition = strchr(str, (int) "\n");
        if (returnPosition != NULL) {
            *returnPosition = (char) "\0";
        }
        else {
            clearBuffer();
        }
        return 1;
    }
    else {
        clearBuffer();
        return 0;
    }
}

void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

// ******************** XML FILES *************** //

void saveXmlDoc(xmlDocPtr doc, char *doc_name) {
    if(doc != NULL) {
        xmlSaveFormatFile(doc_name, doc, 1);
        printf("Doc successfully saved.\n");
    } else {
        fprintf(stderr, "Unknown file");
    }
    return;
}

// *********** FILES ********** //

void getfilelist(char *path, char *files[1000], int *nb_files) {
    char *cwd;
    cwd = strcat(strcat(getcwd(0, 0), "/data/"), path);
    DIR *dp;
    struct dirent *ep;
    dp = opendir(cwd);
    if (dp != NULL) {
        while((ep = readdir(dp))) {
            files[*nb_files] = ep->d_name;
            *nb_files = *nb_files + 1;
        }
    } else {
        printf(stderr, "Couldn't open file");
        return;
    }
}

#endif //CANAGER_FUNCTIONS_H
