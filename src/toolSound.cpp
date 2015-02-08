//
//  toolSound.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 08/02/2015.
//
//

#include "toolSound.h"

//--------------------------------------------------------------
toolSound::toolSound(toolManager* pParent) : tool("Sound", pParent)
{
	mp_soundInput = 0;
}

//--------------------------------------------------------------
void toolSound::createControlsCustom()
{
	if (mp_canvas==0) return;
	
	mp_canvas->addLabel("Sound");
	mp_canvas->addSpacer(300,1);

	if (mp_soundInput)
	{
		ofxUISlider* pSlider = new ofxUISlider("volInputMax", 0.0f, 1.0f, &mp_soundInput->m_volMaxInput,300,20);
		
//		mp_canvas->addSlider("volInputMax", 0.0f, 1.0f, &mp_soundInput->m_volMaxInput);
		mp_canvas->addWidgetDown(pSlider);
		mp_canvas->autoSizeToFitWidgets();
	}
}