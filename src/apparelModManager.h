//
//  apparelModManager.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#pragma once
#include "ofMain.h"

class apparelMod;
class apparelModel;
class user;

class apparelModManager
{
	public:
		apparelModManager			();
	
	
		void						constructMods		(apparelModel* pModel);
		void						addMod				(apparelMod*);
		void						loadModData			();
		void						selectMod			(string name);
		apparelMod*					getMod				(string id);
		apparelMod*					getModCurrent		(){return mp_modCurrent;}
		void						setModel			(apparelModel*);
		void						copyModelToMods		(const apparelModel& model);
 		apparelModel*				getModelLastInChain	();
 		apparelMod*					getModLastInChain	();
 
	
		void						saveModel			();
		void						saveParameters		();
	
		void						deleteMods			();
	
		void						draw				();
		void						drawModsExtra		();
 
 
		// Modifiers to apply, order important
		void						applyModChain		();

		// Count words for a user by looking into database => call in setup when user is created
		void						countUserWords		(user* pUser, bool lock=false);
	
		// Called asynchronously
		void						onNewText			(user* pUser, string text);
		void						onNewWords			(user* pUser, vector<string>& words);

		// Chain of mods
		map<string, apparelMod*>	m_mods;
 		vector<apparelMod*>			m_modsChain;
 		apparelMod*					mp_modCurrent;
 
 		// User associated with this manager
		user*						mp_user;
 
   private:
   		void						makeModsChain		();
};
