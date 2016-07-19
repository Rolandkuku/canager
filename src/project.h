

#ifndef CANAGER_PROJECT_H
#define CANAGER_PROJECT_H

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

// *********** Declarations ******** //

// main
int makingProject();
int projectMenu();
char *chooseProject();
void projectSubMenu();
void createProject();
void addTask();
void xmlParseTask(xmlDocPtr doc, Task *task);
void createProjectFile(char *filename, char *file_path);

// ***************** XML FILES ********** //

void createProjectFile(char *filename, char *file_path) {
    int rc;
    xmlTextWriterPtr writer;
    xmlDocPtr doc;
    xmlChar *tmp;


    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("xmlNewTextWriterDoc: Error creating the xml writer\n");
        return;
    }

    // encoding
    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0) {
        printf("xmlTextWriterStartDocument: Error while encoding xml file.\n");
        return;
    }
    // start root element
    rc = xmlTextWriterStartElement(writer, BAD_CAST "project");
    if (rc < 0) {
        printf("xmlTextWriterStartElement: Error while starting root element.\n");
        return;
    }
    // set project name
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST filename);
    if (rc < 0) {
        printf("xmlTextWriterWriteAttribute: error while setting project name");
    }
    // ned root element
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("xmlTextWriterEndElement: Error while ending root element\n");
        return;
    }
    xmlFreeTextWriter(writer);
    xmlSaveFileEnc(file_path, doc, "UTF-8");
    xmlFreeDoc(doc);
}

void printTask(xmlNodePtr cur) {
    xmlChar *data;
    data = xmlGetProp(cur, (const xmlChar *)"name");
    printf("%s\n", data);
    xmlFree(data);
    return;
}

void listProjects(xmlDocPtr doc) {
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"task"))) {
            printTask(cur);
        }
        cur = cur->next;
    }
}

// ********** Project Menus ******** //

int projectMenu() {
    printf("\n\n\n-- Project Menu --\n");
    printf("1. New Project\n");
    printf("2. New Task\n");
    printf("3. Go back to main menu\n");
    int choice = 0;
    int correct_input = 0;
    scanf("%d", &choice);
    while (correct_input == 0) {
        switch (choice) {
            case 1 :
                createProject();
                correct_input = 1;
                break;
            case 2 :
                addTask();
                correct_input = 1;
                break;
            case 3 :
                return 0;
            default:
                return 0;
        }
    }
}

void projectSubMenu() {
    printf("What do you want to do ?\n");
    printf("1. Make another project.\n");
    printf("2. Go back to project menu.\n");
    int sub_choice = 0;
    scanf("%d", &sub_choice);
    if (sub_choice == 1) {
        createProject();
    } else if (sub_choice == 2) {
        projectMenu();
    }
}

// ********** Project file Creator *******//

void createProject() {
    printf("\n / / ******* New Project ****** / /\n");
    char *filename = malloc(sizeof(char)*50);
    char *file_path;
    printf("Name of the project : \n");
    scanf("%s", filename);
    file_path = strcat(getcwd(0, 0), "/data/projects/");
    file_path = strcat(strcat(file_path, filename), ".xml");

    createProjectFile(filename, file_path);

    projectSubMenu();
}

// ********** Managing project ********* //

char *chooseProject() {
    printf("/*** Which project ? ***/\n");
    char *files[1000];
    int nb_files = 0;
    getfilelist("projects", files, &nb_files);
    for (int i=2; i < nb_files; i++) {
        printf("%d. ", i-2);
        printf("%s\n", files[i]);
    }
    int choice = -1;
    while(choice > nb_files || choice < 0) {
        scanf("%d", &choice);
    }
    return files[choice+2];
}

// ********* Managing Tasks ****** //

void xmlParseTask(xmlDocPtr doc, Task *task) {
    xmlNodePtr cur;
    xmlNodePtr new_node;
    xmlAttrPtr new_attr;
    char *skill = parseSkill(task->skill);
    cur = xmlDocGetRootElement(doc);
    new_node = xmlNewTextChild(cur, NULL, "task", NULL);
    new_attr = xmlNewProp(new_node, "name", (const xmlChar *)task->name);
    new_attr = xmlNewProp(new_node, "desc", (const xmlChar *)task->desc);
    new_attr = xmlNewProp(new_node, "duration", (const xmlChar *)task->duration);
    //new_attr = xmlNewProp(new_node, "skill", (const xmlChar *)skill);
    new_attr = xmlNewProp(new_node, "dependency", (const xmlChar *)task->dependency);
}

void makeTask(Task *task) {
    printf("// ****** Makin Task ***** // \n");
    // Name
    printf("Name : \n");
    scanf("%s", task->name);
    // Description
    printf("Description : \n");
    scanf("%s", task->desc);
    // Duration
    printf("Duration : \n");
    scanf("%s", task->duration);
    // Dependency
    printf("Dependency : \n");
    scanf("%s", task->dependency);
    // Skill
    int skill;
    int correct_input = 0;
    while (correct_input == 0) {
        printf("Which skill ? \n 1. BACK \n 2. FRONT \n 3. SELLS \n");
        clearBuffer();
        scanf("%d", &skill);
        switch (skill) {
            case 1 :
                task->skill = BACK;
                correct_input = 1;
                break;
            case 2 :
                task->skill = FRONT;
                correct_input = 1;
                break;
            case 3 :
                task->skill = SELLS;
                correct_input = 1;
                break;
            default:
                // TODO return err;
                break;
        }
    }
    clearBuffer();
    printf("Task created.\n");
}

void addTask() {
    char *filename = chooseProject();
    printf("Selected project : %s\n", filename);

    xmlDocPtr doc;
    xmlNodePtr cur;
    char * file_path;
    file_path = strcat(strcat(getcwd(0, 0), "/data/projects/"), filename);
    doc =  xmlParseFile(file_path);
    if (doc == NULL) {
        fprintf(stderr, "Doc not parsed successfully. \n");
        return;
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        printf("No root element. \n");
        xmlFreeDoc(doc);
    }

    printf("Tasks already registered : \n\n");
    listProjects(doc);

    Task task;
    makeTask(&task);

    printf("** task created **");

    xmlParseTask(doc, &task);
    saveXmlDoc(doc, file_path);
    printf("Task saved. \n");

    xmlFreeDoc(doc);
}

// ********** Main prog ********* //

int projectProg() {
    projectMenu();
    return 1;
}

#endif //CANAGER_PROJECT_H
