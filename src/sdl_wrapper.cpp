#include "common.h"
#include "sdl_wrapper.h"
#include "sdl_renderer.h"


void LeSdlWrapper::set_rendering_controller(LeSdlWrapper* controller){
	//LOG("set_rendering_controller %p --> %p\n",(void*)this, (void*)controller);
	LOG("set_rendering_controller %p \n",(void*)controller);       
	m_renderer_controller = controller;
}


//static vLeSdlWrapper* get_rendering_controller() {
//    return m_renderer_controller; 
//}

LeSdlWrapper::LeSdlWrapper() { //const char* title) {
	m_render_manager = LeSdlRendererManager::get();
	m_render = m_render_manager->get_renderer();
}

	
void LeSdlWrapper::enter_event_loop() {
	//SDL_Log("hav");
	unsigned int lastTime = 0, currentTime;
	unsigned int last_x = 0;
	unsigned int last_y = 0 ;

	SDL_Event e;
	bool quit = false;
	while( !quit ) {
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 10) {

			while( SDL_PollEvent( &e ) != 0 ) {
				if ( m_renderer_controller ) {
					if ( e.type == SDL_QUIT ) quit = true;
					else if ( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) m_renderer_controller->notify_mouse_pressed(1);
					else if ( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT )m_renderer_controller->notify_mouse_pressed(0);
					else if ( e.type == SDL_MOUSEMOTION  ) {
						last_x = e.motion.x;
						last_y = e.motion.y;
						m_renderer_controller->notify_mouse_move(last_x,last_y);
					}
					//if ( e.type == SDL_MOUSEWHEEL)
					//if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) 
					//if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) 
				}
			}
			//SDL_Log("hatuk");
			//get_active_rnderer(
			//LOG("this %p m_renderer_controller %p \n",(void*)this,(void*)m_renderer_controller);
			if(m_renderer_controller) {
				//SDL_Log("?????\n");
				m_render_manager->scene_clear();
				//m_render_manager->render_background_image();
				m_renderer_controller->update(currentTime);
				m_renderer_controller->draw();
				m_render_manager->scene_draw();
			}
			lastTime = currentTime;  
		}
	}
}



void LeSdlWrapper::draw_point(unsigned int y, unsigned int x, unsigned int radius) { 
	draw_circle(y,x,radius);
}

void LeSdlWrapper::set_drawing_color(int r, int g, int b) {
    SDL_SetRenderDrawColor(m_render,r,g,b,255);
}


void LeSdlWrapper::set_background_image(const char* ipath) {
    register_image(ipath);
    auto info = m_render_manager->get_image_info(ipath);
    m_bg_texture = info.first; 
    m_bg_texture_width = info.second.w;
    m_bg_texture_height = info.second.h;
    
}

void LeSdlWrapper::draw_static_background() {
        SDL_Rect srect;
        srect.x = 0;
        srect.y = 0;
        srect.w = scr_w();
        srect.h = scr_h();
        SDL_RenderCopy(m_render, m_bg_texture, &srect, NULL);
}

bool LeSdlWrapper::draw_scroll_background(unsigned int delta) {	
    //m_render_manager->set_background_image(path);
	//FIXME background texture constant and have set_background_image 
    
    //unsigned int height = scr_h();
    if ( delta + scr_w() < m_bg_texture_width ) {
        SDL_Rect srect;
        srect.x = delta;
        srect.y = 0;
        srect.w = scr_w();
        srect.h = scr_h();
        SDL_RenderCopy(m_render, m_bg_texture, &srect, NULL);
    } else {
        unsigned int diff1 = m_bg_texture_width - delta;
        //unsigned int diff2 = scr_w() - delta;
       
        SDL_Rect drect1;
        drect1.x = 0;
        drect1.y = 0;
        drect1.w = diff1;
        drect1.h = scr_h();
        
        
        SDL_Rect srect1;
        srect1.x = delta;
        srect1.y = 0;
        srect1.w = diff1;
        srect1.h = scr_h();
        

        SDL_Rect drect2;
        drect2.x = diff1;
        drect2.y = 0;
        drect2.w = scr_w();
        drect2.h = scr_h();
        
        
        SDL_Rect srect2;
        srect2.x = 0;
        srect2.y = 0;
        srect2.w = diff1;
        srect2.h = scr_h();

        SDL_RenderCopy(m_render, m_bg_texture, &srect1, &drect1);
        SDL_RenderCopy(m_render, m_bg_texture, NULL, &drect2);
        if ( diff1 == 0 )
            return true;
    }

    return false;
    
}

void LeSdlWrapper::draw_text(const char* s, unsigned int y, unsigned int x, unsigned int sy, unsigned int sx)
{
	m_render_manager->draw_text(s,y,x,sy,sx);
}


//todo more clever way?
std::pair<std::string, std::pair<unsigned int,unsigned int> > LeSdlWrapper::register_image(const char* ipath) {
    return m_render_manager->register_image(ipath);
}

void LeSdlWrapper::draw_image(const char* ipath, unsigned int y, unsigned int x, unsigned int cropw, unsigned int croph) {
	//return;
    std::pair<SDL_Texture*,SDL_Rect> info = m_render_manager->get_image_info(ipath);
	
	SDL_Rect irect;
	irect.x = x;
	irect.y = y;
	irect.w = info.second.w/cropw;
	irect.h = info.second.h/croph;

	SDL_RenderCopy(m_render, info.first, NULL, &irect);
}

void LeSdlWrapper::draw_image(const char* ipath, unsigned int y, unsigned int x, unsigned int cropw, unsigned int croph, double angle, bool needflip, uint flipmode) {
	//return;
    std::pair<SDL_Texture*,SDL_Rect> info = m_render_manager->get_image_info(ipath);
	
	//fixme get rid of this temp. variables
	SDL_Rect irect;
	irect.x = x;
	irect.y = y;
	irect.w = info.second.w/cropw;
	irect.h = info.second.h/croph;

	if ( needflip ) {
		SDL_RenderCopyEx(m_render, info.first, NULL, &irect, angle, NULL , SDL_FLIP_NONE );	
		
		return;
	}
	
	SDL_RenderCopyEx(m_render, info.first, NULL, &irect, angle, NULL, SDL_FLIP_NONE );	
}


void LeSdlWrapper::draw_square(unsigned int y, unsigned int x, unsigned int delta) {
	draw_rect(y-delta,x-delta,2*delta,2*delta);
}  

void LeSdlWrapper::draw_rect(unsigned int y, unsigned int x, unsigned int delta2, unsigned int delta1 ) {
	SDL_Rect rectToDraw = {x,y,delta1,delta2};
	SDL_RenderDrawRect(m_render,&rectToDraw);
	SDL_RenderFillRect(m_render, &rectToDraw);
}  

void LeSdlWrapper::draw_line(unsigned int y1, unsigned int x1, unsigned int y2, unsigned int x2) {
	SDL_RenderDrawLine(m_render,x1,y1,x2,y2);
}


void LeSdlWrapper::draw_circle(unsigned int y, unsigned int x, int radius)
{
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(m_render, x + dx, y + dy);
            }
        }
    }
}

