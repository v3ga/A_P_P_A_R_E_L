//
//  apparelModLoader.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/01/2016.
//
//

#pragma once
#include "ofMain.h"

class apparelModLoader
{
	public:
		apparelModLoader			();
	
		void			load		();
		void			update		(float dt);
		void			draw		();

		void			setVisible	(bool is=true){m_bVisible = is;}
		void			setPosition	(ofVec3f p){m_position = p;}
		void			setPosition	(float x, float y, float z){m_position.set(x,y,z);}
		void			setWidth	(float w);
	
	
	private:
		ofImage			m_image;
		ofVec3f			m_position;
		ofVec2f			m_size;
		float			m_rotation, m_rotationSpeed;
		bool			m_bVisible;


};