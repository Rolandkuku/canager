
#ifndef CANAGER_PLANNING_H
#define CANAGER_PLANNING_H

#include <libxml/HTMLtree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

/**
 * Return a team file path
 */
char *selectFile(char *entity) {
    int nb_files = 1;
    char * files[100];
    char * cwd;
    char * selectHint;
    if (strcmp(entity, "teams") == 0) {
        cwd = strcat(getcwd(0, 0), "/data/teams/");
        selectHint = "Select a team :\n";
    } else if (strcmp(entity, "projects") == 0) {
        cwd = strcat(getcwd(0, 0), "/data/projects/");
        selectHint = "Select a project :\n";
    } else {
        fprintf(stderr, "Param must be either 'teams' or 'projects'");
        return NULL;
    }
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
    printf(selectHint);
    readInput(chosenTeam, 2);
    long filePosition = strtol(chosenTeam, NULL, 10);
    free(chosenTeam);
    char *chosenFile = files[filePosition + 3];

    return strcat(cwd, chosenFile);
}

/**
 * Return number of teammates for a given team
 */
int getNbTeammates(xmlDocPtr doc) {
    int nb_teammates = 0;
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"teammate"))) {
            nb_teammates++;
        }
        cur = cur->next;
    }
    return nb_teammates;
}

int getNbTasks(xmlDocPtr doc) {
    int nb_tasks = 0;
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"task"))) {
            nb_tasks++;
        }
        cur = cur->next;
    }
    return nb_tasks;
}

/**
 * Fill the teammate according to cursor position.
 */
void getTeammate(xmlNodePtr cur,  Teammate *teammate) {
    xmlChar *data;
    data = xmlGetProp(cur, (const xmlChar *)"first_name");
    strcpy(teammate->first_name, (const char *)data);
    data = xmlGetProp(cur, (const xmlChar *)"last_name");
    strcpy(teammate->last_name, (char *)data);
    data = xmlGetProp(cur, (const xmlChar *)"email");
    strcpy(teammate->email, (char *)data);
    data = xmlGetProp(cur, (const xmlChar *)"skill");
    teammate->skill = unParseSkill((char *)data);
    xmlFree(data);
}

void getTask(xmlNodePtr cur, Task *task) {
    xmlChar *data;
    data = xmlGetProp(cur, (const xmlChar *)"name");
    strcpy(task->name, (const char *) data);
    data = xmlGetProp(cur, (const xmlChar *)"desc");
    strcpy(task->desc, (const char *) data);
    data = xmlGetProp(cur, (const xmlChar *)"duration");
    strcpy(task->duration, (const char *) data);
    data = xmlGetProp(cur, (const xmlChar *)"dependency");
    strcpy(task->dependency, (const char *) data);
    data = xmlGetProp(cur, (const xmlChar *)"skill");
    task->skill = unParseSkill((char *)data);
    task->finishedBy = 0;
    xmlFree(data);
}

/**
 * Fill an array with teammates.
 */
void getTeammates(xmlDocPtr doc, Teammate *teammates) {
    int i = 0;
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"teammate"))) {
            getTeammate(cur, &teammates[i]);
            i++;
        }
        cur = cur->next;
    }
}

/**
 * Fill an array with tasks.
 */
void getTasks(xmlDocPtr doc, Task *tasks) {
    int i = 0;
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while(cur != NULL) {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"task"))) {
            getTask(cur, &tasks[i]);
            i++;
        }
        cur = cur->next;
    }
}

void parsePlanning(xmlDocPtr doc, Planning planning) {
    xmlNodePtr  cur;
    xmlNodePtr new_task;
    cur = xmlDocGetRootElement(doc);
    for (int i = 0; i < planning.nb_tasks; i++) {
        new_task = xmlNewTextChild(cur, NULL, (const xmlChar *)"li", (const xmlChar *)planning.tasks[i].name);
    }
}

/**
 * Parse all plannings
 */
void parsePlannings(xmlDocPtr doc, Planning plannings[], int nb_plannings) {

    xmlNodePtr cur;
    for (int i = 0; i < nb_plannings; i++) {
        cur = xmlDocGetRootElement(doc);
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))){
                xmlNodePtr new_list;
                xmlNewTextChild(cur, NULL, (const xmlChar *)"h4", (const xmlChar *)plannings[i].teammate_last_name);
                new_list = xmlNewTextChild(cur, NULL, (const xmlChar *)"ul", NULL);
                if (plannings[i].nb_tasks == 0) {
                    xmlNewTextChild(new_list, NULL, (const xmlChar *)"li", (const xmlChar *)"This user has no task");
                } else {
                    for (int j = 0; j < plannings[i].nb_tasks; j++) {
                        xmlNodePtr new_task;
                        char fb[6];
                        char * style[200];
                        strcat(style, "border:1px solid #000000; padding-left:");
                        sprintf(fb, "%d", plannings[i].tasks[j].finishedBy);
                        strcat(style, (const char *) fb);
                        strcat(style, "0px;");
                        new_task = xmlNewTextChild(
                                new_list, NULL, (const xmlChar *)"li",
                                (const xmlChar *)strcat(
                                        strcat(
                                                strcat(
                                                        strcat(plannings[i].tasks[j].name, " - "),
                                                        (const char *) plannings[i].tasks[j].desc),
                                               " - Estimated end : "),
                                        (const char *) fb));
                        //strcat(strcat("border:1px solid #000000; margin-right:", strcat(fb, 0)), "px;");
                        xmlNewProp(
                                new_task,
                                (const xmlChar *)"style",
                                (const xmlChar *)style
                        );
                        xmlNewProp(
                                new_task,
                                (const xmlChar *)"data-duration",
                                (const xmlChar *)plannings[i].tasks[j].duration
                        );
                        xmlNewProp(
                                new_task,
                                (const xmlChar *)"data-estimatedend",
                                (const xmlChar *)fb
                        );
                    }
                }
            }
            cur = cur->next;
        }
    }
}

/**
 * Create planning from team and project
 */
void planningSecretReciep(Teammate *teammates, int nb_teammates, Task *tasks, int nb_tasks) {
    //int nb_teammates = sizeof(teammates)/ sizeof(teammates[0]);
    //int nb_tasks= sizeof(tasks)/ sizeof(tasks[0]);
    int count_tasks = nb_tasks;
    char *tasks_done[nb_tasks];
    for (int o = 0; o < nb_tasks; o++) {
        tasks_done[o] = "";
    }
    int nb_tasks_done = 0;
    int time = 0;
    Planning plannings[nb_teammates];
    // Initialize plannings
    for (int i = 0; i < nb_teammates; i++) {
        strcpy(plannings[i].teammate_last_name, teammates[i].last_name);
        plannings[i].nb_tasks = 0;
        for (int j = 0; j < 100; j++) {
            plannings[i].tasks[j].finishedBy = 0;
        }
    }

    while (count_tasks > 0) {
        // iterate over all teammates
        for (int i = 0; i < nb_teammates; i++) {
            // Iterate over all tasks
            for (int j = 0; j < nb_tasks; j++) {
                printf("%s\n", tasks[j].dependency);
                for (int o = 0; o < nb_tasks; o++) {
                }
                // Is the tasks assignable ?
                if ((strcmp(tasks[j].dependency, "none") == 0 && in_array(tasks[j].name, tasks_done, nb_tasks) == 0) ||
                    (in_array(tasks[j].dependency, tasks_done, nb_tasks) == 1) && in_array(tasks[j].name, tasks_done, nb_tasks) == 0) {
                    // Is the teammate skilled for the task ?
                    if (strcmp(parseSkill(tasks[j].skill), parseSkill(teammates[i].skill)) == 0) {
                        for (int k = 0; k < nb_teammates; k++) {
                            // Looking for the current teammate in the plannings array
                            if (strcmp(teammates[i].last_name, plannings[k].teammate_last_name) == 0) {
                                int teammate_available = 1;
                                // Looping over current teammate tasks
                                int nb_current_teammate_tasks =
                                        sizeof(plannings[k].tasks) / sizeof(plannings[k].tasks[0]);
                                for (int l = 0; l < nb_current_teammate_tasks; l++) {
                                    // Is the current teammate available ?
                                    if (plannings[k].tasks[l].finishedBy > time) {
                                        teammate_available = 0;
                                        break;
                                    }
                                }
                                // if the current teammate is available, we dance 'til we die
                                if (teammate_available == 1) {
                                    // Add task name to tasks done
                                    tasks_done[nb_tasks_done] = tasks[j].name;
                                    nb_tasks_done++;
                                    // Set finish date
                                    int finished = time + (tasks[j].duration[0] - '0');
                                    tasks[j].finishedBy = finished;
                                    //tasks[j].time = sprintf()
                                    // Copy task to current teammate planning
                                    plannings[k].tasks[plannings[k].nb_tasks] = tasks[j];
                                    plannings[k].nb_tasks++;
                                    // Adjust tasks left
                                    count_tasks--;
                                }
                            }
                        }
                    }
                }
            }
        }
        time++;
    }
    for (int i = 0; i < nb_teammates; i ++) {
        for (int j = 0; j < plannings[i].nb_tasks; j++) {
            printf("Task assigned : %s | task duration : %d\n", plannings[i].tasks[j].name, plannings[i].tasks[j].finishedBy);
        }
    }

    char * file_name = malloc(sizeof(char)*51);
    char * file_path;
    printf("Name of the planning : \n");
    scanf("%s", file_name);
    file_path = strcat(strcat(strcat(getcwd(0, 0), "/data/plannings/"), file_name), ".html");
    xmlChar xml_file_path = (xmlChar)file_path;
    xmlDocPtr doc;
    xmlTextWriterPtr writer;
    int rc;

    // ** Create planning doc ** //
    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("Create planning file: Error creating the xml writer\n");
        return;
    }
    // HTML5 DOCTYPE
    xmlTextWriterWriteDTD(writer, (xmlChar *)"html", NULL, NULL, NULL);
    // start <html> root element
    rc = xmlTextWriterStartElement(writer, BAD_CAST "html");
    if (rc < 0) {
        printf("<html> root element start: Error at xmlTextWriterStartElement\n");
        return;
    }
    // start <head> element
    rc = xmlTextWriterStartElement(writer, BAD_CAST "head");
    if (rc < 0) {
        printf("<head> element start: Error at xmlTextWriterStartElement\n");
        return;
    }
    // end <head> element
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("<head> element end: Error at xmlTextWriterStartElement\n");
        return;
    }
    // start <body> element
    rc = xmlTextWriterStartElement(writer, BAD_CAST "body");
    if (rc < 0) {
        printf("<head> element start: Error at xmlTextWriterStartElement\n");
        return;
    }
    // end <body> element
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("<head> element end: Error at xmlTextWriterStartElement\n");
        return;
    }
    // end <html> root element
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("<html> root element end: Error at xmlTextWriterStartElement\n");
        return;
    }
    xmlFreeTextWriter(writer);

    // ** Parsing planning ** //
    parsePlannings(doc, plannings, nb_teammates);
    htmlSaveFileFormat(file_path, doc, "UTF-8", NULL);

    xmlFreeDoc(doc);

}

/**
 * Main planning prog
 */
void makePlanning() {
    char *team_path = selectFile("teams");
    xmlDocPtr doc;
    doc = xmlParseFile(team_path);
    if (doc == NULL) {
        fprintf(stderr, "Doc not parsed successfully. \n");
    }
    int nb_teammates = getNbTeammates(doc);
    Teammate teammates[nb_teammates];
    getTeammates(doc, teammates);

    printf("Available teammates :\n");
    for (int i =0; i < nb_teammates; i++) {
        printf("%s\n", teammates[i].first_name);
    }

    char *project_path = selectFile("projects");
    doc = xmlParseFile(project_path);
    if (doc == NULL) {
        fprintf(stderr, "Doc not parsed successfully. \n");
    }
    int nb_tasks = getNbTasks(doc);
    Task tasks[nb_tasks];
    getTasks(doc, tasks);

    printf("Tasks for this projects : \n");
    for (int i = 0; i < nb_tasks; i++) {
        printf("%s\n", tasks[i].name);
    }

    planningSecretReciep(teammates, nb_teammates, tasks, nb_tasks);
}


// ** Menus ** //

void planningMenu() {
    printf("1. Make new planning.\n");
    printf("2. Go back to main menu.\n");
    int sub_choice = 0;
    scanf("%d", &sub_choice);
    if (sub_choice == 1) {
        makePlanning();
    } else if (sub_choice == 2) {
        return;
    }
}

// ** Main prog ** //

void planningProg() {
    planningMenu();
}

#endif //CANAGER_PLANNING_H


