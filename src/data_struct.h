//
// Created by roland on 28/04/16.
//

typedef enum Skill Skill;
enum Skill {
   BACK, FRONT
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
    int id;
    char *first_name;
    char *last_name;
    char *email;
    int team_id;
    Skill skill;
};

