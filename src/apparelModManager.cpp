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
		mod->setOscSender( (oscSenderInterface*) GLOBALS->getOscSender() );
		m_mods[mod->m_id] = mod;
		m_modsChain.push_back(mod);
	}
	//makeModsChain();
}


//--------------------------------------------------------------
void apparelModManager::loadModData()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it)
	{
		it->second->loadModel();
		it->second->loadParameters();
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
void apparelModManager::copyModelToMods(const apparelModel& model)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		apparelMod* pMod = it->second;
		pMod->copyModelFrom(model);
	}
}

//--------------------------------------------------------------
apparelModel* apparelModManager::getModelLastInChain()
{
	int nb = m_modsChain.size();
	if (nb>0)
		return &m_modsChain[nb-1]->m_model;
	return 0;
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
	int nbMods = m_modsChain.size();
	if (nbMods>=1)
	{
		m_modsChain[nbMods-1]->draw();
	}
}

//--------------------------------------------------------------
void apparelModManager::drawModsExtra()
{
	int nbModsChains = m_modsChain.size();
	for (int i=0;i<nbModsChains;i++)
	{
		m_modsChain[nbMods-1]->drawExtra();
	}
}

//--------------------------------------------------------------
void apparelModManager::applyModChain()
{
	int nbMods = m_modsChain.size();
	
	// Copy if something changed at some point
	for (int i=0; i<nbMods; i++)
	{
		// Something changed in the model (faces/vertices selection)
		if (m_modsChain[i]->isChanged())
		{
			// Recompute mod's parameters
			// May be vertices / faces were added, proper flag should be set
			m_modsChain[i]->apply();

			// Copy the «new» model to the next mod in the chain
			if (i<nbMods-1/* && m_modsChain[i]->isMeshChanged()*/)
			{
				m_modsChain[i+1]->copyModelFrom( m_modsChain[i]->m_model );
			}
		}
	}


	// Reset flags
	for (int i=0; i<nbMods; i++)
	{
		m_modsChain[i]->setChanged(false);
		m_modsChain[i]->setMeshChanged(false);
	}

	// perform update
	for (int i=0; i<nbMods; i++)
	{
	  m_modsChain[i]->update();
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

//--------------------------------------------------------------
void apparelModManager::makeModsChain()
{
/*	m_modsChain.clear();
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it)
*/
}



