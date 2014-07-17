//
//  apparelModManager.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#include "apparelModManager.h"
#include "apparelMod.h"
#include "globals.h"

apparelModManager::apparelModManager()
{
	mp_modCurrent = 0;
}

//--------------------------------------------------------------
void apparelModManager::addMod(apparelMod* mod)
{
	if (mod)
	{
		mod->loadParameters();
		mod->setOscSender( GLOBALS->getOscSender() );
		m_mods[mod->m_id] = mod;
	}
}

//--------------------------------------------------------------
void apparelModManager::setModel(apparelModel* pModel)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->mp_model = pModel;
	}
}


//--------------------------------------------------------------
void apparelModManager::selectMod(string name)
{
	mp_modCurrent = m_mods[name];
}


//--------------------------------------------------------------
void apparelModManager::saveParameters()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->saveParameters();
		delete it->second;
	}
}

