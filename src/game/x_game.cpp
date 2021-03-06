#include "x_game.h"
#include "game_state.h"
#include "menu_state.h"
#include "resources.h"
#include "../skeleton/state_manager.h"

#ifdef OS_WINDOWS
    #define RESPATH "C:\\Users\\elen\\Desktop\\sdl_game\\ForLevonjanik\\Debug\\"
#elif OS_ANDROID
    #define RESPATH ""
#else
    #define RESPATH "./bin/"
#endif


XGame::XGame() {
	m_state_mgr = LeStateManager::get();
    m_resources = LeResourceManager::get();

    m_resources->set_mus_path(RESPATH);
    m_resources->set_img_path(RESPATH);
    m_resources->set_snd_path(RESPATH);
}


void XGame::init() {
    register_images();

    m_state_mgr->reg_state(ST_GAME,new LeGameState);
    m_state_mgr->reg_state(ST_MENU,new LeMenuState);
    
}

void XGame::start() {
	//todo check for some conditions
	m_state_mgr->set_start_state(ST_GAME);
}

void XGame::stop() {
	//save as ?
}

void XGame::register_images() {
    register_resources();
}



