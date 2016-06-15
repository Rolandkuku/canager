//
// Created by Roland Kullet on 02/05/16.
//

#ifndef CANAGER_FUNCTIONS_H
#define CANAGER_FUNCTIONS_H

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

#endif //CANAGER_FUNCTIONS_H
