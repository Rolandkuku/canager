//
// Created by roland on 28/04/16.
//

typedef enum Skill Skill;
enum Skill {
   BACK, FRONT, SELLS
};

typedef struct Task Task;
struct Task {
    int id;
    char *name;
    char *desc;
    int duration;
    Skill skill;
    int depedency_id;
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

