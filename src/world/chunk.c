#include "chunk.h"
#include "../util.h"
#include "../vec.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "block/block.h"

chunk_t chunk_gen(int x, int y, int z) {
    chunk_t chunk = {0};
    chunk.x = x;
    chunk.y = y;
    chunk.z = z;
    chunk.indexes_count = 1;
    chunk.vertexes_count = 1;
    if (y/CHUNK_Y == -1) {
        for (int x = 0; x < CHUNK_X; x++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                chunk.blocks[x][CHUNK_Y-1][z] = (block_t){.id = STUD};
            }
        }
    }
    chunk_bake(&chunk);
    return chunk;
}

static void push_vertex(void *args, vertex_t vertex) {
    chunk_t *chunk = (chunk_t *)args;
    chunk->vertexes_size++;
    while (chunk->vertexes_count <= chunk->vertexes_size) {
        chunk->vertexes_count <<= 1;
        chunk->vertexes = srealloc(chunk->vertexes, chunk->vertexes_count * sizeof(vertex_t));
    }
    chunk->vertexes[chunk->vertexes_size - 1] = vertex;
}

static void push_index(void *args, unsigned int index) {
    chunk_t *chunk = (chunk_t *)args;
    chunk->indexes_size++;
    while (chunk->indexes_size >= chunk->indexes_count) {
        chunk->indexes_count <<= 1;
        chunk->indexes = srealloc(chunk->indexes, chunk->indexes_count * sizeof(unsigned int));
    }
    chunk->indexes[chunk->indexes_size - 1] = index;
}

static void push_blockinfo(chunk_t *chunk, ivec3 info) {
    chunk->blockinfo_size++;
    while (chunk->blockinfo_size >= chunk->blockinfo_count) {
        chunk->blockinfo_count <<= 1;
        chunk->blockinfo = srealloc(chunk->blockinfo, chunk->blockinfo_count * sizeof(ivec3));
    }
    memcpy(&chunk->blockinfo[chunk->blockinfo_size - 1], info, sizeof(ivec3));
}

static void set_face(chunk_t *chunk, int x, int y, int z, enum Face face) {
    vec2 scale, uv;
    block_get_uv(chunk->blocks[x][y][z], face, &scale, &uv);

    blockmesh_push_face(
        push_vertex,
        push_index,
        chunk,
        chunk, 
        &chunk->vertexes_size,
        scale,
        uv,
        face,
        x, 
        y,
        z
    );
    for (int i = 0; i < 4; i++) push_blockinfo(chunk, (ivec3){chunk->x + x, y, chunk->z + z});
}

void chunk_bake(chunk_t *chunk) {
    // TODO: subbuffer
    free(chunk->vertexes);
    free(chunk->indexes);
    free(chunk->blockinfo);
    chunk->vertexes = NULL;
    chunk->indexes = NULL;
    chunk->blockinfo = NULL;
    chunk->vertexes_size = 0;
    chunk->indexes_size = 0;
    chunk->blockinfo_size = 0;
    chunk->indexes_count = 1;
    chunk->vertexes_count = 1;
    chunk->blockinfo_count = 1;
    if (!chunk->vao.handle) chunk->vao = vao_create();
    if (!chunk->vbo.handle) chunk->vbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    if (!chunk->ebo.handle) chunk->ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    if (!chunk->bbo.handle) chunk->bbo = vbo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

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
    vbo_buffer(&chunk->bbo, chunk->blockinfo, 0, chunk->blockinfo_size * sizeof(ivec3));
}

void chunk_bake_at(chunk_t *chunk, int x, int y, int z) {
    // TODO:
    // fking implement this
    chunk_bake(chunk);
}

void chunk_draw(chunk_t *chunk) {
    renderer_chunk(&state.renderer, 
        chunk->vao,
        chunk->vbo,
        chunk->ebo,
        chunk->bbo,
        (vec3){
            chunk->x,
            chunk->y,
            chunk->z
    }, RENDERER_TEXTURE_BLOCKATLAS);
}
