#include "blockmesh.h"

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
) {
    vertex_t vertex[4];
    switch (face) {
        case FACE_FRONT:
            vertex[0] = (vertex_t){{x,   y, z+1}, {0, 0}};
            vertex[1] = (vertex_t){{x,   y+1,z+1}, {1, 0}};
            vertex[2] = (vertex_t){{x+1, y+1,z+1}, {1, 1}};
            vertex[3] = (vertex_t){{x+1, y, z+1}, {0, 1}};
            break;
        case FACE_BACK:
            vertex[0] = (vertex_t){{x,   y,   z}, {0, 0}};
            vertex[1] = (vertex_t){{x+1, y,   z}, {1, 0}};
            vertex[2] = (vertex_t){{x+1, y+1, z}, {1, 1}};
            vertex[3] = (vertex_t){{x,   y+1, z}, {0, 1}};
            break;
        case FACE_RIGHT:
            vertex[0] = (vertex_t){{x+1, y,   z},   {0, 0}};
            vertex[1] = (vertex_t){{x+1, y,   z+1}, {1, 0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1, 1}};
            vertex[3] = (vertex_t){{x+1, y+1, z},   {0, 1}};
            break;
        case FACE_LEFT:
            vertex[0] = (vertex_t){{x, y,   z+1}, {0, 0}};
            vertex[1] = (vertex_t){{x, y,   z},   {1, 0}};
            vertex[2] = (vertex_t){{x, y+1, z},   {1, 1}};
            vertex[3] = (vertex_t){{x, y+1, z+1}, {0, 1}};
            break;
        case FACE_TOP:
            vertex[0] = (vertex_t){{x,   y+1, z},   {0, 0}};
            vertex[1] = (vertex_t){{x+1, y+1, z},   {1, 0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1, 1}};
            vertex[3] = (vertex_t){{x,   y+1, z+1}, {0, 1}};
            break;
        case FACE_BOTTOM:
            vertex[0] = (vertex_t){{x,   y, z+1}, {0, 0}};
            vertex[1] = (vertex_t){{x+1, y, z+1}, {1, 0}};
            vertex[2] = (vertex_t){{x+1, y, z},   {1, 1}};
            vertex[3] = (vertex_t){{x,   y, z},   {0, 1}};
    }

    for (int i = 0; i < 4; i++) {
        glm_vec2_mul(vertex[i].texture, scale, vertex[i].texture);
        glm_vec2_add(vertex[i].texture, uv, vertex[i].texture);
        push_vertex(push_vertex_args, vertex[i]);
    }

    const size_t start = *vertex_size - 4;
    push_index(push_index_args, start + 0);
    push_index(push_index_args, start + 1);
    push_index(push_index_args, start + 2);
    push_index(push_index_args, start + 2);
    push_index(push_index_args, start + 3);
    push_index(push_index_args, start + 0);
}

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
) {
    for (enum Face face = 0; face < FACE_LAST; face++) {
        blockmesh_push_face(push_vertex, push_index, push_vertex_args, push_index_args, vertex_size, scale, uv, face, x, y, z);
    }
}

void blockmesh_remove(RemoveVertex remove_vertex, RemoveIndex remove_index, size_t vertex_index, size_t indice_index) {
    remove_vertex(vertex_index, 4 * 6);
    remove_index(indice_index, 6);
}
