
#ifndef CANAGER_PLANNING_H
#define CANAGER_PLANNING_H


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

/**
 * Create planning from team and project
 */
void planningSecretReceip(Teammate *teammates, int nb_teammates, Task *tasks, int nb_tasks) {
    //int nb_teammates = sizeof(teammates)/ sizeof(teammates[0]);
    //int nb_tasks= sizeof(tasks)/ sizeof(tasks[0]);
    int count_tasks = nb_tasks;
    char *tasks_done[nb_tasks];
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
        printf("entering while\n");
        // iterate over all teammates
        for (int i = 0; i < nb_teammates; i++) {
            printf("looping over teammates \n");
            // Iterate over all tasks
            for (int j = 0; j < nb_tasks; j++) {
                // Is the tasks assignable ?
                if ((strcmp(tasks[j].dependency, "none") == 0 || in_array(tasks[j].dependency, tasks_done) == 1)
                    && in_array(tasks[j].name, tasks_done) == 0) {
                    printf("Task assignable\n");
                    // Is the teammate skilled for the task ?
                    printf("teammate skill : %s | teammate name : %s\n", parseSkill(teammates[i].skill), teammates[i].last_name);
                    printf("task skill : %s | task name : %s\n", parseSkill(tasks[j].skill), tasks[j].name);
                    if (strcmp(parseSkill(tasks[j].skill), parseSkill(teammates[i].skill)) == 0) {
                        printf("The teammate has the skill we need\n");
                        for (int k = 0; k < nb_teammates; k++) {
                            // Looking for the current teammate in the plannings array
                            if (strcmp(teammates[i].last_name, plannings[k].teammate_last_name) == 0) {
                                printf("found the teammate in plannings\n");
                                int teammate_available = 1;
                                // Looping over current teammate tasks
                                int nb_current_teammate_tasks =
                                        sizeof(plannings[k].tasks) / sizeof(plannings[k].tasks[0]);
                                printf("nb teammate tasks : %d\n", nb_current_teammate_tasks);
                                for (int l = 0; l < nb_current_teammate_tasks; l++) {
                                    // Is the current teammate available ?
                                    if (plannings[k].tasks[l].finishedBy > time) {
                                        printf("KESSTUFOULA ?\n");
                                        teammate_available = 0;
                                        break;
                                    }
                                }
                                // if the current teammate is available, we dance 'til we die
                                if (teammate_available == 1) {
                                    printf("Teammate is available\n");
                                    printf("assigning task\n");
                                    // Add task name to tasks done
                                    tasks_done[nb_tasks_done] = tasks[j].name;
                                    nb_tasks_done++;
                                    // Set finish date
                                    tasks[j].finishedBy = time + (tasks[j].duration[0] - '0');
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
        printf("Teammate name : %s\n", plannings[i].teammate_last_name);
        for (int j = 0; j < plannings[i].nb_tasks; j++) {
            printf("Task assigned : %s | task duration : %d\n", plannings[i].tasks[j].name, plannings[i].tasks[j].finishedBy);
        }
    }
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

    planningSecretReceip(teammates, nb_teammates, tasks, nb_tasks);
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


