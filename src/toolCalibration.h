//
//  toolCalibration.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#pragma once
#include "tool.h"

class toolCalibration : public tool
{
	public:
		toolCalibration			(toolManager* parent);

		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);
	
};
