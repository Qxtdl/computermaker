#include "../global.h"
#include "../util.h"
#include "../state.h"
#include "block/block.h"
#include "wire.h"
#include "world.h"

extern wire_t *wires;
extern int wires_size;

typedef struct {
    enum BlockId id;
    enum LogicState state, new_state;
} saveblock_t;

typedef struct {
    int x, 
        y, // futureproof
        z;
    saveblock_t blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
} savechunk_t;

#define savechunk_terminator ((savechunk_t){ \
    .x = 0xdeadc0de, \
    .y = 0xcafebabe, \
    .z = 0xf00fc7c8, \
    .blocks = {0} \
})

typedef struct {
    int ox, oy, oz,
        dx, dy, dz
} savewire_t;

#define savewire_terminator ((savewire_t){ \
    .ox = 0xdeadc0de, \
    .oy = 0xcafebabe, \
    .oz = 0xf000000f, \
    .dx = 0, \
    .dy = 0, \
    .dz = 0 \
})

int save_load(const char *filename) {
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) {
        app_warn("Failed to load the world save \"%s\". Continuing as normal\n", filename)
        return 0;
    }

    void *save = readfile(filename),
        *_save = save;
    // there is a save header, skip ahead
    while (*((char *)save++));


    savechunk_t *chunks = save;
    for (int i = 0;; i++) {
        savechunk_t savechunk = chunks[i];
        if (!memcmp(&savechunk, &savechunk_terminator, sizeof(savechunk_t))) {
            save = &chunks[i + 1];
            break;
        }

        chunk_t chunk = {0};
        chunk.x = savechunk.x;
        chunk.z = savechunk.z;
        chunk.indexes_count = 1;
        chunk.vertexes_count = 1;
        
        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    saveblock_t saveblock = savechunk.blocks[x][y][z];

                    chunk.blocks[x][y][z] = (block_t){
                        .id = savechunk.blocks[x][y][z].id,
                        .gate = (logic_gate_t){
                            .state = saveblock.state,
                            .new_state = saveblock.new_state
                        }
                    };
                }
            }
        }

        chunk_bake(&chunk);
        world_add_chunk(&state.world, chunk);
    }

    savewire_t *wires = save;
    for (int i = 0;; i++) {
        savewire_t wire = wires[i];
        if (!memcmp(&wire, &savewire_terminator, sizeof(savewire_t)))
            break;

        world_create_wire((wire_t){
            .ox = wire.ox,
            .oy = wire.oy,
            .oz = wire.oz,
            .dx = wire.dx,
            .dy = wire.dy,
            .dz = wire.dz,
        });
    }

    free(_save);

    app_warn("Save \"%s\" loaded\n", filename)

    return 1;
}

void save_save(const char *filename) {
    FILE *fptr = fopen(filename, "wb");
    if (!fptr) {
        app_warn("Failed to save the world\n")
        return;
    }

    #define header "Save file for ComputerMaker " APP_RELEASE_STRING
    fwrite(header, 1, strlen(header) + 1, fptr);
    #undef header

    struct world *world = &state.world;
    for (size_t i = 0; i < state.world.chunks_size; i++) {
        savechunk_t savechunk = {0};
        savechunk.x = world->chunks[i].x;
        savechunk.y = 0;
        savechunk.z = world->chunks[i].z;

        for (int x = 0; x < CHUNK_X; x++) {
            for (int y = 0; y < CHUNK_Y; y++) {
                for (int z = 0; z < CHUNK_Z; z++) {
                    block_t block = world->chunks[i].blocks[x][y][z];
                    if (block.id == AIR) continue;

                    savechunk.blocks[x][y][z] = (saveblock_t){
                        .id = block.id,
                        .state = block.gate.state,
                        .new_state = block.gate.new_state
                    };
                }
            }
        }

        fwrite(&savechunk, sizeof(savechunk_t), 1, fptr);
    }

    fwrite(&savechunk_terminator, sizeof(savechunk_t), 1, fptr);

    for (int i = 0; i < wires_size; i++) {
        fwrite(&(savewire_t){
            .ox = wires[i].ox,
            .oy = wires[i].oy,
            .oz = wires[i].oz,
            .dx = wires[i].dx,
            .dy = wires[i].dy,
            .dz = wires[i].dz,
        }, sizeof(savewire_t), 1, fptr);
    }

    fwrite(&savewire_terminator, sizeof(savewire_t), 1, fptr);

    fclose(fptr);

    app_warn("The world was saved to %s\n", filename)
}
