//
//  toolMods.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#pragma once

#include "tool.h"
#include "apparelMod.h"
#include "apparelModUI.h"

class apparelModel;
class apparelModManager;
class toolMods : public tool
{
	public:
		toolMods				(toolManager* parent, apparelModManager* modManager, apparelModel* model);
		~toolMods				();

		void					show				(bool is);
		bool					isHit				(int x, int y);
		void					exit				();
	
		void					createControlsCustom();
		void					createControlsCustomFinalize();
		void					handleEvents		(ofxUIEventArgs& e);
		apparelModUI*			makeInstanceModUI	(apparelMod*);
		void					selectMod			(string name);
	
	
		apparelModManager*				mp_modsManager;
		apparelModUI*					mp_modUICurrent;
		apparelModel*					mp_apparelModel;

		map<string, apparelModUI*>		m_modsUI;
};
