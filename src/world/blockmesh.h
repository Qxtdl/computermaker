#pragma once

#include "../gfx/vertex.h"

enum Face {
    FACE_TOP,
    FACE_BOTTOM,
    FACE_RIGHT,
    FACE_LEFT,
    FACE_FRONT,
    FACE_BACK,
    FACE_LAST
};

typedef void (*PushVertex)(void *args, vertex_t vertex);
typedef void (*PushIndex)(void *args, unsigned int index);
typedef void (*RemoveVertex)(size_t index, size_t n);
typedef void (*RemoveIndex)(size_t index, size_t n);

void blockmesh_push_face(
    PushVertex push_vertex, 
    PushIndex push_index, 
    void *push_vertex_args, 
    void *push_index_args, 
    size_t *vertex_size,
    vec2 scale,
    vec2 uv,
    enum Face face,
    float x, 
    float y, 
    float z
);
void blockmesh_push(
    PushVertex push_vertex, 
    PushIndex push_index,
    void *push_vertex_args,
    void *push_index_args,
    size_t *vertex_size,
    vec2 scale,
    vec2 uv,    
    float x,
    float y,
    float z
);
void blockmesh_remove(RemoveVertex remove_vertex, RemoveIndex remove_index, size_t vertex_index, size_t indice_index);
