//
// Created by roland on 28/04/16.
//

typedef enum Skill Skill;
enum Skill {
   BACK, FRONT, SELLS
};

typedef struct Task Task;
struct Task {
    char name[51];
    char desc[256];
    int duration;
    Skill skill;
    char dependency[51];
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

