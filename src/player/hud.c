#include "../global.h"
#include "../state.h"
#include "../gfx/renderer.h"

void hud_draw(void) {
    glDisable(GL_DEPTH_TEST);

    char buf[256];
    snprintf(buf, sizeof(buf), 
        "Selected block: \n"
        "%s \n"
        "(q to change block)\n"
        "Mode: %s\n"
        "(e to cycle)", 
        block_id_name(state.player.selected_block),
        mode_name(state.player.mode)
    );
    renderer_text(0, 32, 2, buf);

    if (state.player.hovered_block &&
    	state.player.hovered_block->id != AIR) {
    	snprintf(buf, sizeof(buf),
    		"Block hovered:\n" 
    		"Type: %s\n"
    		"Inputs: %d\n"
    		"State: %d\n"
    		"New State: %d\n",
    		block_id_name(state.player.hovered_block->id),
    		state.player.hovered_block->gate.num_inputs,
    		state.player.hovered_block->gate.state,
    		state.player.hovered_block->gate.new_state
    	);
    	renderer_text(0, 224, 2, buf);
    }
    
    renderer_text(0, 0, 2, APP_RELEASE_STRING);
}
