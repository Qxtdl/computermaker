#include "../global.h"
#include "../config.h"
#include "../state.h"
#include "../gfx/renderer.h"
#include "hud.h"
#include "chat.h"

static char text_buffer[512];

float fontscale = 2;

void hud_init(void) {
    fontscale = atoi(config_get("FONTSCALE"));
}

void hud_draw(void) {
    glDisable(GL_DEPTH_TEST);

    snprintf(text_buffer, sizeof(text_buffer),
        "Block: %s\n"
        "Mode: %s\n"
		"(Q - change block)\n"
		"(E - change mode)\n"
		"(R - reset camera position)\n"
		"(Z - save world)\n"
		"(O - toggle wireframe)\n"
		"(F11 - fullscreen toggle)\n",
        block_id_name(state.player.selected_block),
        mode_name(state.player.mode)
    );

    renderer_text(0, 32, fontscale, text_buffer, NULL);
  
    if (state.player.hovered_block && state.player.hovered_block->id != AIR) {
    	snprintf(text_buffer, sizeof(text_buffer),
    		"ID: %s\n"
    		"Inputs: %d\n"
    		"State: %d\n"
    		"New State: %d\n",
    		block_id_name(state.player.hovered_block->id),
    		state.player.hovered_block->gate.num_inputs,
    		state.player.hovered_block->gate.state,
    		state.player.hovered_block->gate.new_state
    	);

    	renderer_text(0, 400, fontscale, text_buffer, NULL);
    }
    
    renderer_text(0, 0, fontscale, APP_RELEASE_STRING, (vec3){0, 1, 0});
    static const char *watermark = NULL;
    if (!watermark) {
        watermark = config_get("SHOW_WATERMARK");
        if (!watermark) { // NOTE: this will not print the message because config_get auto exits if there is no key
            app_error("SHOW_WATERMARK must exist and be y/n\n")
        }
    }

    #define watermark_text "https://github.com/Qxtdl/computermaker"

    if (*watermark == 'y')
        // NOTE: Properly size and multiply text by scale?
    	renderer_text((int)(window.width / 2) - (int)(strlen(watermark_text) * 10), 0, fontscale, watermark_text, NULL);

    render_chat();        
}
