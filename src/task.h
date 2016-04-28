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
    char name[sizeof(char)*30];
    char desc[sizeof(char)*512];
    int duration;
    Skill skill;
    Task dependency;
};

