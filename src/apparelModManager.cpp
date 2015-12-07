//
//  apparelModManager.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#include "apparelModManager.h"
#include "apparelMod.h"
#include "apparelMod_include.h"
#include "globals.h"
#include "user.h"

apparelModManager::apparelModManager()
{
	mp_modCurrent 	= 0;
	mp_moodCurrent	= 0;

	m_bModForceWeightAutomatic = false;
}

//--------------------------------------------------------------
void apparelModManager::applyModForceWeightAutomatic()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it)
	{
		it->second->setForceWeightAutomatic( m_bModForceWeightAutomatic );
	}
}


//--------------------------------------------------------------
void apparelModManager::constructMods(apparelModel* pModel)
{
	deleteMods();

	// ——————————————————————————————————————————————————————
	// MODS
	addMod( new apparelMod_authoritopathy() );
	addMod( new apparelMod_pedopathy() );
	addMod( new apparelMod_sportopathy() );
	addMod( new apparelMod_selfopathy() );
	addMod( new apparelMod_zoopathy() );
	addMod( new apparelMod_pretentiopathy() );
	addMod( new apparelMod_meteopathy() );
	addMod( new apparelMod_kawaiopathy() );
	
	// used for requests in sql database : DO NOT CHANGE
	getMod("Authoritopathy")	->setId(0);
	getMod("Pedopathy")			->setId(1);
	getMod("Sportopathy")		->setId(2);
	getMod("Selfopathy")		->setId(3);
	getMod("Zoopathy")			->setId(4);
	getMod("Pretentiopathy")	->setId(5);
	getMod("Meteopathy")		->setId(6);
	getMod("Kawaiopathy")		->setId(7);
	
	// ——————————————————————————————————————————————————————
	// MOODS
	addMood	( new apparelMood_porcupinopathy() );
	addMood	( new apparelMood_noisopathy() );
	addMood	( new apparelMood_sad() );

//	selectMood( "noisopathy" );

	GLOBALS->mp_modSelfopathy = (apparelMod_selfopathy*)getMod("Selfopathy");

	copyModelToMods(*pModel);
	applyModForceWeightAutomatic();
	loadModData();
	loadMoodData();
	applyModChain();
}

//--------------------------------------------------------------
void apparelModManager::addMod(apparelMod* mod)
{
	if (mod)
	{
		mod->createParameters();
		mod->setOscSender( (oscSenderInterface*) GLOBALS->getOscSender() );
		m_mods[mod->m_id] = mod;
		m_modsChain.push_back(mod);
	}
	//makeModsChain();
}

//--------------------------------------------------------------
void apparelModManager::addMood(apparelMod* pMood)
{
	if (pMood)
	{
		pMood->createParameters();
		pMood->setOscSender( (oscSenderInterface*) GLOBALS->getOscSender() );
		m_moods[pMood->m_id] = pMood;
	}
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
void apparelModManager::loadMoodData()
{
	map<string, apparelMod*>::iterator it;
	for (it = m_moods.begin(); it != m_moods.end(); ++it)
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
		//it->second->mp_model = pModel;
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
apparelMod*	apparelModManager::getModLastInChain()
{
	int nb = m_modsChain.size();
	if (nb>0)
		return m_modsChain[nb-1];
	return 0;
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
void apparelModManager::selectMood(string name)
{
	unselectMood();

	if (name !="")
	{
		int nbModsChain = m_modsChain.size();

		if (nbModsChain>0)
		{
			mp_moodCurrent = m_moods[name];
			if (mp_moodCurrent)
			{
				m_modsChain.push_back( mp_moodCurrent );
				mp_moodCurrent->copyModelFrom( m_modsChain[nbModsChain-1]->m_model );
				mp_moodCurrent->setChanged(); // to be sure
			}
		}
	}
}

//--------------------------------------------------------------
void apparelModManager::unselectMood()
{
	if (mp_moodCurrent)
	{
		m_modsChain.pop_back();
		mp_moodCurrent = 0;
	}
}

//--------------------------------------------------------------
apparelMod* apparelModManager::getMod(string name)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		if (it->second->m_id == name && it->second->isMood()==false)
			return it->second;
	}

	return 0;
}

//--------------------------------------------------------------
apparelMod* apparelModManager::getMood(string name)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_moods.begin(); it != m_moods.end(); ++it){
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

	for (it = m_moods.begin(); it != m_moods.end(); ++it){
		it->second->saveParameters();
	}


}

//--------------------------------------------------------------
void apparelModManager::deleteMods()
{
	selectMood(""); // remove mood from chain

	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		delete it->second;
	}

	for (it = m_moods.begin(); it != m_moods.end(); ++it){
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
//		m_modsChain[nbModsChains-1]->drawExtra();
		m_modsChain[i]->drawExtra();
	}
}

//--------------------------------------------------------------
void apparelModManager::applyModChain()
{
//	OFAPPLOG->begin("apparelModManager::applyModChain");
	int nbMods = m_modsChain.size();
	
	// Copy if something changed at some point
	for (int i=0; i<nbMods; i++)
	{
//		OFAPPLOG->println("["+ofToString(i)+"] "+m_modsChain[i]->getId());
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
	
//	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelModManager::countUserWords(user* pUser, bool lock)
{
	int nbMods = m_modsChain.size();
	
	// Copy if something changed at some point
	for (int i=0; i<nbMods; i++)
	{
		m_modsChain[i]->countUserWords(pUser, lock);
	}

}


//--------------------------------------------------------------
void apparelModManager::onNewText(user* pUser, string text)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->onNewText(pUser, text);
	}
}

//--------------------------------------------------------------
void apparelModManager::onNewWords(user* pUser, vector<string>& words)
{
	map<string, apparelMod*>::iterator it;
	for (it = m_mods.begin(); it != m_mods.end(); ++it){
		it->second->onNewWords(pUser, words);
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



