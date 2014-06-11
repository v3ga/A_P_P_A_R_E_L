//
//  toolModel.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 30/05/2014.
//
//

#pragma once

#include "tool.h"

class toolModel : public tool
{
	public:
	
		toolModel			(toolManager* parent);

		void				createControlsCustom();
		void				handleEvents		(ofxUIEventArgs& e);

};
