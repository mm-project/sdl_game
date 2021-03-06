#ifndef image_object_h
#define image_object_h

#include "../renderer/scene_controller.h"

#include "mover.h"
#include "resource_manager.h"
#include "common.h"

class LeImageObject 
{
    public:
        
        LeImageObject(int image_id, unsigned int posx, unsigned int posy, unsigned int cropx, unsigned int cropy) {
            m_obj = LeObj(get_rsc(image_id),posx,posy,cropx,cropy);
        }
        void set_destination(unsigned int x, unsigned int y) {
            m_obj.m_x = x;
            m_obj.m_y = y;
        }

    public:
        void draw_obj_static() {
            get_renderer()->draw_image(m_obj.m_img_path.c_str(),m_obj.m_x,m_obj.m_y,m_obj.m_c_x,m_obj.m_c_y,m_obj.m_angle,m_obj.m_need_flip,0);//obj.m_flip_mode);
            
        }
        
        void draw_obj_in_movement2() {
            get_renderer()->draw_image(m_obj.m_img_path.c_str(),m_obj.m_old_x,m_obj.m_old_y,m_obj.m_c_x,m_obj.m_c_y,m_obj.m_angle,m_obj.m_need_flip,0);//obj.m_flip_mode);
        }

        //void set_mover(LeObjMoverBase& m) {
         //   m_mover
        //}
    //private:
        LeObj m_obj;
        //LeObjMover<MV_STRAIGHT> m_mover;
};



#endif
