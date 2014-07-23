//
//  toolUser.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#pragma once
#include "tool.h"

class user;
class toolUser : public tool
{
	public:
		toolUser				(toolManager* parent, user* pUser);

		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);

	protected:
		user*					mp_user;
		ofxUILabel*				mp_lblTitle;
	
};