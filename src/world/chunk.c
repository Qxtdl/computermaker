#include "chunk.h"
#include "../util.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "block/block.h"

chunk_t chunk_gen(int x, int z) {
    chunk_t chunk = {0};
    chunk.x = x;
    chunk.z = z;
    chunk.indexes_count = 1;
    chunk.vertexes_count = 1;
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < 1; y ++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                chunk.blocks[x][y][z] = (block_t){.id = BRICK};
            }
        }
    }
    chunk_bake(&chunk);
    return chunk;
}

static void push_vertex(chunk_t *chunk, vertex_t vertex) {
    chunk->vertexes_size++;
    while (chunk->vertexes_count <= chunk->vertexes_size) {
        chunk->vertexes_count <<= 1;
        chunk->vertexes = srealloc(chunk->vertexes, chunk->vertexes_count * sizeof(vertex_t));
    }
    chunk->vertexes[chunk->vertexes_size - 1] = vertex;
}

static void push_index(chunk_t *chunk, unsigned int indice) {
    chunk->indexes_size++;
    while (chunk->indexes_size >= chunk->indexes_count) {
        chunk->indexes_count <<= 1;
        chunk->indexes = srealloc(chunk->indexes, chunk->indexes_count * sizeof(unsigned int));
    }
    chunk->indexes[chunk->indexes_size - 1] = indice;
}

static void set_face(chunk_t *chunk, int x, int y, int z, enum Face face) {
    vec2 scale, uv;
    block_get_uv(chunk->blocks[x][y][z], face, &scale, &uv);

    vertex_t vertex[4];
    switch (face) {
        case FACE_FRONT:
            vertex[0] = (vertex_t){{x,   y,   z+1}, {0, 0}};
            vertex[1] = (vertex_t){{x+1, y,   z+1}, {1, 0}};
            vertex[2] = (vertex_t){{x+1, y+1, z+1}, {1, 1}};
            vertex[3] = (vertex_t){{x,   y+1, z+1}, {0, 1}};
            break;
        case FACE_BACK:
            vertex[0] = (vertex_t){{x+1, y,   z}, {0, 0}};
            vertex[1] = (vertex_t){{x,   y,   z}, {1, 0}};
            vertex[2] = (vertex_t){{x,   y+1, z}, {1, 1}};
            vertex[3] = (vertex_t){{x+1, y+1, z}, {0, 1}};
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
        push_vertex(chunk, vertex[i]);
    }

    const size_t start = chunk->vertexes_size - 4;
    push_index(chunk, start + 0);
    push_index(chunk, start + 1);
    push_index(chunk, start + 2);
    push_index(chunk, start + 2);
    push_index(chunk, start + 3);
    push_index(chunk, start + 0);    
}

void chunk_bake(chunk_t *chunk) {
    free(chunk->vertexes);
    free(chunk->indexes);
    chunk->vertexes = NULL;
    chunk->indexes = NULL;
    chunk->indexes_count = 1;
    chunk->vertexes_count = 1;
    vao_destroy(chunk->vao);
    vbo_destroy(chunk->vbo);
    vbo_destroy(chunk->ebo);
    chunk->vao = vao_create();
    chunk->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    chunk->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                if (chunk->blocks[x][y][z].id == AIR)
                    continue;

                if (x == CHUNK_X - 1 || chunk->blocks[x + 1][y][z].id == AIR) // right
                    set_face(chunk, x, y, z, FACE_RIGHT);
                if (x == 0 || chunk->blocks[x - 1][y][z].id == AIR) // left
                    set_face(chunk, x, y, z, FACE_LEFT);
                if (y == CHUNK_Y - 1 || chunk->blocks[x][y + 1][z].id == AIR) // top
                    set_face(chunk, x, y, z, FACE_TOP);
                if (y == 0 || chunk->blocks[x][y - 1][z].id == AIR) // bottom
                    set_face(chunk, x, y, z, FACE_BOTTOM);
                if (z == CHUNK_Z - 1 || chunk->blocks[x][y][z + 1].id == AIR) // front
                    set_face(chunk, x, y, z, FACE_FRONT);
                if (z == 0 || chunk->blocks[x][y][z - 1].id == AIR) // back
                    set_face(chunk, x, y, z, FACE_BACK);
            }
        }
    }

    vao_bind(chunk->vao);
    vbo_buffer(&chunk->vbo, chunk->vertexes, 0, chunk->vertexes_size * sizeof(vertex_t));
    vbo_buffer(&chunk->ebo, chunk->indexes, 0, chunk->indexes_size * sizeof(unsigned int));
}

void chunk_bake_at(chunk_t *chunk, int x, int y, int z) {
    // TODO:
    // fking implement this
    chunk_bake(chunk);
}

void chunk_draw(chunk_t *chunk) {
    renderer_mesh(&state.renderer, 
        chunk->vao,
        chunk->vbo,
        chunk->ebo,
        (vec3){
            chunk->x,
            0,
            chunk->z
    }, RENDERER_TEXTURE_BLOCKATLAS);
}

