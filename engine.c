#include "engine.h"

void timer_start(Timer *timer, float lifetime) {
    if (timer == NULL) {
        printf("LOG: timer_start() Timer was NULL");
        return;
    }

    timer->lifetime = lifetime;
}

void timer_update(Timer *timer) {

    // subtract this frame from the timer if its not already expired
    if (timer == NULL && timer->lifetime <= 0) {
        return;
    }

    timer->lifetime -= GetFrameTime();
}

bool timer_is_done(Timer *timer) { 
    if (timer == NULL) {
        printf("LOG: timer_is_done() Timer was NULL");
        return false;
    }

    return timer->lifetime <= 0;
}

void update_camera(Camera2D *camera, Entity *entity, float delta_time, int width, int height) {
    camera->offset = (Vector2){width/2 , height/2};
    camera->target = entity->pos;
}

