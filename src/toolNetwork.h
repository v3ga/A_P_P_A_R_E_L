//
//  toolNetwork.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#pragma once
#include "tool.h"

class oscSenderInterface;
class toolNetwork : public tool
{
	public:
		toolNetwork				(toolManager* parent);


		void					createControlsCustom	();
		void					updateUI				();
		void					handleEvents			(ofxUIEventArgs& e);

	protected:
		ofxUILabel*				mp_lblOscSending;
		oscSenderInterface*		mp_oscSender;
};
