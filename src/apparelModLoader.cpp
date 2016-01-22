//
//  apparelModLoader.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/01/2016.
//
//

#include "apparelModLoader.h"


//--------------------------------------------------------------
apparelModLoader::apparelModLoader()
{
	m_bVisible = false;
	m_rotationSpeed = 30.0f;
}

//--------------------------------------------------------------
void apparelModLoader::setWidth(float w)
{
	float r = m_image.getWidth() / m_image.getHeight();

	m_size.x = w;
	m_size.y = w / r;
}


//--------------------------------------------------------------
void apparelModLoader::load()
{
	m_image.load("images/loader2.png");
	m_image.setAnchorPercent(0.5,0.5);
}

//--------------------------------------------------------------
void apparelModLoader::update(float dt)
{
	if (m_bVisible)
	{
		m_rotation += m_rotationSpeed*dt;
		if (m_rotation>=360.0f) m_rotation -= 360.0f;
	}
}


//--------------------------------------------------------------
void apparelModLoader::draw()
{
	if (m_bVisible)
	{
		ofPushMatrix();
		ofTranslate(m_position);
		ofRotateX(90);
		ofRotate(-m_rotation);
		m_image.draw(0,0,m_size.x,m_size.y);
		ofPopMatrix();
	}
}





