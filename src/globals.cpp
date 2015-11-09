//
//  globals.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#include "globals.h"

globals* globals::instance = 0;

globals::globals()
{
	pOfApp				= 0;
	pOscSender			= 0;
	m_soundInputVolume	= 0.0f;
	pUser				= 0;
	pModel				= 0;
	
	mp_modSelfopathy = 0;
}

globals* globals::getInstance()
{
	if (instance == 0)
		instance = new globals();
	return instance;
}