//
// Created by roland on 28/04/16.
//


// **** Skill **** //
typedef enum Skill Skill;
enum Skill {
   BACK, FRONT, SELLS
};

char *parseSkill(Skill skill) {
    switch (skill) {
        case FRONT :
            return "FRONT";
        case BACK :
            return "BACK";
        case SELLS :
            return "SELLS";
        default:
            printf((const char *) stderr, "Wrong skill");
            return "//";
    }
}

Skill unParseSkill(char * skill) {
    if(strcmp(skill, "FRONT") == 0) {
        return FRONT;
    } else if (strcmp(skill, "BACK") == 0) {
        return BACK;
    } else if (strcmp(skill, "SELLS") == 0) {
        return SELLS;
    }
}

typedef struct Task Task;
struct Task {
    char name[51];
    char desc[256];
    char duration[6];
    int finishedBy;
    char dependency[51];
    Skill skill;
};

typedef struct Team Team;
struct Team {
    int id;
    char *name;
};

typedef struct Teammate Teammate;
struct Teammate {
    char first_name[51];
    char last_name[51];
    char email[51];
    Skill skill;
};

typedef struct Planning Planning;
struct Planning {
    char teammate_last_name[51];
    Task tasks[100];
    int nb_tasks;
};

