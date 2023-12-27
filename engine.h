#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "raylib.h"
#include "raymath.h"

typedef struct Entity {
    unsigned int id;
    Vector2 pos;
} Entity;

typedef struct Timer {
    float lifetime;
} Timer;

void load_textures();
void load_sounds();
void startup();

void timer_start(Timer *timer, float lifetime);
void timer_update(Timer *timer);
bool timer_is_done(Timer *timer);

void update_camera(Camera2D *camera, Entity *entity, float delta_time, int width, int height);
