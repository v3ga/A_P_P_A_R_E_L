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
		mod->setOscSender( (oscSenderInterface*) GLOBALS->getOscSender() );
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
apparelModel* apparelModManager::getModelToDraw()
{
	int nbModsOrdered = m_modsOrdered.size();
	if (nbModsOrdered>=1)
	{
		return m_modsOrdered[nbModsOrdered-1]->mp_modelChain;
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
	}
}

//--------------------------------------------------------------
void apparelModManager::saveParameters()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->saveParameters();
	}
}

//--------------------------------------------------------------
void apparelModManager::deleteMods()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		delete it->second;
	}
}

//--------------------------------------------------------------
void apparelModManager::draw()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->draw();
	}
}

//--------------------------------------------------------------
void apparelModManager::addChain(apparelMod* pMod)
{
	m_modsOrdered.push_back(pMod);
}

//--------------------------------------------------------------
void apparelModManager::applyChain()
{
	int nbMods = m_modsOrdered.size();
	if (nbMods>=1)
	{
		apparelMod* pPreviousMod = 0;
		for (int i=0;i<nbMods;i++)
		{
			m_modsOrdered[i]->apply(pPreviousMod);
			pPreviousMod = m_modsOrdered[i];
		}
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



