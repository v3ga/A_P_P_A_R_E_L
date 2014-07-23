//
//  toolCalibration.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#include "toolCalibration.h"

//--------------------------------------------------------------
toolCalibration::toolCalibration(toolManager* parent) : tool("Calibration", parent)
{
}

//--------------------------------------------------------------
void toolCalibration::createControlsCustom()
{
	if (mp_canvas==0) return;

	mp_canvas->addLabel("Calibration");
	mp_canvas->addSpacer(300,1);

	mp_canvas->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void toolCalibration::handleEvents(ofxUIEventArgs& e)
{
}
