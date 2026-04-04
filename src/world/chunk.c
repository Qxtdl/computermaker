#include "chunk.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "block.h"

chunk_t chunk_gen(int x, int z) {
    chunk_t chunk = {0};
    chunk.x = x;
    chunk.z = z;
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y ++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                chunk.blocks[x][y][z] = (block_t){.type = BLOCK_TYPE_VISIBLE, .texture = RENDERER_TEXTURE_STUD};
            }
        }
    }
    chunk_bake(&chunk);
    return chunk;
}

static void push_vertex(chunk_t *chunk, vertex_t vertex) {
    chunk->vertices = srealloc(chunk->vertices, ++chunk->vertices_size * sizeof(vertex_t));
    chunk->vertices[chunk->vertices_size - 1] = vertex;
}

static void push_indice(chunk_t *chunk, unsigned int indice) {
    chunk->indices = srealloc(chunk->indices, ++chunk->indices_size * sizeof(unsigned int));
    chunk->indices[chunk->indices_size - 1] = indice;
}

enum Face {
    FACE_TOP,
    FACE_BOTTOM,
    FACE_RIGHT,
    FACE_LEFT,
    FACE_FRONT,
    FACE_BACK,
};

static void set_face(chunk_t *chunk, int x, int y, int z, enum Face face) {
    vertex_t vertex[4];
    switch (face) {
        case FACE_FRONT:
            vertex[0] = (vertex_t){{x,   y,   z+1}, {0,0}};
            vertex[1] = (vertex_t){{x+1, y,   z+1}, {1,0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1,1}};
            vertex[3] = (vertex_t){{x,   y+1, z+1}, {0,1}};
            break;
        case FACE_BACK:
            vertex[0] = (vertex_t){{x+1, y,   z}, {0,0}};
            vertex[1] = (vertex_t){{x,   y,   z}, {1,0}};
            vertex[2] = (vertex_t){{x,   y+1, z}, {1,1}};
            vertex[3] = (vertex_t){{x+1, y+1, z}, {0,1}};
            break;
        case FACE_RIGHT:
            vertex[0] = (vertex_t){{x+1, y,   z},   {0,0}};
            vertex[1] = (vertex_t){{x+1, y,   z+1}, {1,0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1,1}};
            vertex[3] = (vertex_t){{x+1, y+1, z},   {0,1}};
            break;
        case FACE_LEFT:
            vertex[0] = (vertex_t){{x, y,   z+1}, {0,0}};
            vertex[1] = (vertex_t){{x, y,   z},   {1,0}};
            vertex[2] = (vertex_t){{x, y+1, z},   {1,1}};
            vertex[3] = (vertex_t){{x, y+1, z+1}, {0,1}};
            break;
        case FACE_TOP:
            vertex[0] = (vertex_t){{x,   y+1, z},   {0,0}};
            vertex[1] = (vertex_t){{x+1, y+1, z},   {1,0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1,1}};
            vertex[3] = (vertex_t){{x,   y+1, z+1}, {0,1}};
            break;
        case FACE_BOTTOM:
            vertex[0] = (vertex_t){{x,   y, z+1}, {0,0}};
            vertex[1] = (vertex_t){{x+1, y, z+1}, {1,0}};
            vertex[2] = (vertex_t){{x+1, y, z},   {1,1}};
            vertex[3] = (vertex_t){{x,   y, z},   {0,1}};
    }

    for (int i = 0; i < 4; i++)
        push_vertex(chunk, vertex[i]);

    const size_t start = chunk->vertices_size - 4;
    push_indice(chunk, start + 0);
    push_indice(chunk, start + 1);
    push_indice(chunk, start + 2);
    push_indice(chunk, start + 2);
    push_indice(chunk, start + 3);
    push_indice(chunk, start + 0);    
}

void chunk_bake(chunk_t *chunk) {
    chunk->vao = vao_create();
    chunk->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    chunk->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                if (chunk->blocks[x][y][z].type == BLOCK_TYPE_INVISIBLE)
                    continue;

                if (x == CHUNK_X - 1 || chunk->blocks[x + 1][y][z].type == BLOCK_TYPE_INVISIBLE) // right
                    set_face(chunk, x, y, z, FACE_RIGHT);
                if (x == 0 || chunk->blocks[x - 1][y][z].type == BLOCK_TYPE_INVISIBLE) // left
                    set_face(chunk, x, y, z, FACE_LEFT);
                if (y == CHUNK_Y - 1 || chunk->blocks[x][y + 1][z].type == BLOCK_TYPE_INVISIBLE) // top
                    set_face(chunk, x, y, z, FACE_TOP);
                if (y == 0 || chunk->blocks[x][y - 1][z].type == BLOCK_TYPE_INVISIBLE) // bottom
                    set_face(chunk, x, y, z, FACE_BOTTOM);
                if (z == CHUNK_Z - 1 || chunk->blocks[x][y][z + 1].type == BLOCK_TYPE_INVISIBLE) // front
                    set_face(chunk, x, y, z, FACE_FRONT);
                if (z == 0 || chunk->blocks[x][y][z - 1].type == BLOCK_TYPE_INVISIBLE) // back
                    set_face(chunk, x, y, z, FACE_BACK);
            }
        }
    }

    vao_bind(chunk->vao);
    vbo_buffer(&chunk->vbo, chunk->vertices, 0, chunk->vertices_size * sizeof(vertex_t));
    vbo_buffer(&chunk->ebo, chunk->indices, 0, chunk->indices_size * sizeof(unsigned int));
}

void chunk_draw(chunk_t *chunk) {
    renderer_box(&state.renderer, 
        chunk->vao,
        chunk->vbo,
        chunk->ebo,
        (vec3){
        chunk->x,
        0,
        chunk->z
    }, RENDERER_TEXTURE_STUD);
}