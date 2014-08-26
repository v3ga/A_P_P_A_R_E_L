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
		mod->loadModel();
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
apparelMod* apparelModManager::getMod(string name)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		if (it->second->m_id == name)
			return it->second;
	}

	return 0;
}


//--------------------------------------------------------------
void apparelModManager::saveModel()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it)
	{
		it->second->saveModel();
		delete it->second;
	}
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

//--------------------------------------------------------------
void apparelModManager::onNewText(string text)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->onNewText(text);
	}
}

//--------------------------------------------------------------
void apparelModManager::onNewWords(vector<string>& words)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->onNewWords(words);
	}
}



