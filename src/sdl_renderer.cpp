#include "sdl_renderer.h"
#include "common.h"

#include <string>
#include <map>
#include <utility>

#include <stdio.h>


LeSdlRendererManager* LeSdlRendererManager::get() {
	if ( ! m_instance ) m_instance = new LeSdlRendererManager();
	return m_instance;
}

//TODO game title from outside

LeSdlRendererManager::LeSdlRendererManager(){ //const char* title) {
	m_init_success = true;
	init("Game demo");
}


SDL_Renderer* LeSdlRendererManager::get_renderer() {
	return m_render;
}


bool  LeSdlRendererManager::init_sdl_ttf() {
#ifdef TEXT_RENDER
	if(!TTF_WasInit() && TTF_Init()==-1) {
		SDL_ShowSimpleMessageBox(0, "TTF_Init: %s\n", TTF_GetError(), NULL);
		m_init_success = false;
		//exit(1);
	}

	m_ttf_font = TTF_OpenFont("FreeSans.ttf", 50); 
	if(!m_ttf_font) {  
		SDL_ShowSimpleMessageBox(0, "TTF_OpenFont: %s\n", TTF_GetError(),NULL);
		m_init_success = false;
		//exit(1);
	}
#endif	
}

bool LeSdlRendererManager::init_sdl_image() {
#ifdef IMAGE_RENDER
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		SDL_ShowSimpleMessageBox(0, "SDL_image could not initialize! SDL_image Error:", IMG_GetError(), NULL);
		m_init_success = false;
	}
#endif
}

bool LeSdlRendererManager::init_sdl() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		SDL_ShowSimpleMessageBox( 0, "SDL could not initialize! SDL Error: %s\n", SDL_GetError(), NULL );
		m_init_success = false;
	}
}

bool LeSdlRendererManager::create_window(const char* title) {
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( m_window == NULL ) 	{
		SDL_ShowSimpleMessageBox(0, "Window could not be created! SDL Error: %s\n", SDL_GetError(),NULL);
		m_init_success = false;
	} else {
		//Create renderer for window
		m_render = SDL_CreateRenderer( m_window, -1, 0 );
		if( m_render == NULL ) 	{
			SDL_ShowSimpleMessageBox(0, "Renderer could not be created! SDL Error: %s\n", SDL_GetError(),NULL );
			m_init_success = false;
		} else {
			SDL_RenderSetScale(m_render,1,1);	
			if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
				LOG( "Warning: Linear texture filtering not enabled!" );
		}			
	}
}

//TODO seperate each init to different inits. 
bool LeSdlRendererManager::init(const char* title)
{
	LOG("init start\n");
	
	init_sdl();
	init_sdl_image();
	init_sdl_ttf();
	create_window(title);

return m_init_success;
}


void LeSdlRendererManager::close()
{
	//Destroy window        
	SDL_DestroyRenderer( m_render );
	SDL_DestroyWindow( m_window );
	m_window = NULL;
	m_render = NULL;

	//Quit SDL subsystems
#ifdef IMAGE_RENDER	
	IMG_Quit();
#endif	
	SDL_Quit();
}


void LeSdlRendererManager::scene_clear() {
	//LOG("SDL render: clear \n");
	SDL_SetRenderDrawColor(m_render,0,0,0,255);
	SDL_RenderClear( m_render );
	SDL_SetRenderDrawColor(m_render,0,0,0,255);
}

//TODO redudant set render color?
void LeSdlRendererManager::scene_draw() {
	//LOG("SDL render: draw exact \n");
	SDL_SetRenderDrawColor( m_render, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderPresent( m_render );
}


void LeSdlRendererManager::set_background_image(const char* path) {
	SDL_Surface* background = IMG_Load(path);
	if(background == NULL) {
		SDL_ShowSimpleMessageBox(0, "Background init error", SDL_GetError(), m_window);
	}

	if(m_render == NULL) {
		SDL_ShowSimpleMessageBox(0, "Renderer init error", SDL_GetError(), m_window);
	}

	m_bg_texture = SDL_CreateTextureFromSurface(m_render,background);
	if(m_bg_texture  == NULL) {
		SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), m_window);
	}
	
	//SDL_RenderCopy(m_render, m_bg_texture, NULL, NULL);
	//delete background;

}

void LeSdlRendererManager::render_background_image() {
	SDL_RenderCopy(m_render, m_bg_texture, NULL, NULL);

}

void LeSdlRendererManager::draw_text(const char* s, unsigned int y, unsigned int x, unsigned int sy, unsigned int sx)
{
#ifdef TEXT_RENDER
	if(!m_ttf_font) 
		return;
	
	SDL_Point p;
	p.x=x;
	p.y=y;
	SDL_Color c = {255, 255, 255};  
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_ttf_font, s , c); 
	SDL_Texture* Message = SDL_CreateTextureFromSurface(m_render, surfaceMessage); 

	SDL_Rect Message_rect; 
	Message_rect.x = p.x;  
	Message_rect.y = p.y; 
	Message_rect.w = sy; 
	Message_rect.h = sx; 

	SDL_RenderCopy(m_render, Message, NULL, &Message_rect); 
#endif		
}



