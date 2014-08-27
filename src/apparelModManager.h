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
class apparelModManager
{
	public:
		apparelModManager			();
	
	
		void						addMod				(apparelMod*);
		void						selectMod			(string name);
		apparelMod*					getMod				(string id);
		apparelMod*					getModCurrent		(){return mp_modCurrent;}
		void						setModel			(apparelModel*);
	
		void						saveModel			();
		void						saveParameters		();
	
		void						deleteMods			();
	
		void						draw				();
	
		// Called asynchronously
		void						onNewText			(string text);
		void						onNewWords			(vector<string>& words);
	
		map<string, apparelMod*>	m_mods;
		apparelMod*					mp_modCurrent;
};
