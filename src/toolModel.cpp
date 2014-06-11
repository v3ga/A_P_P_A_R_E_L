//
//  toolModel.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 30/05/2014.
//
//

#include "toolModel.h"

//--------------------------------------------------------------
toolModel::toolModel(toolManager* parent) : tool("Models", parent)
{
}


//--------------------------------------------------------------
void toolModel::createControlsCustom()
{
	if (mp_canvas)
	{
	}
}

//--------------------------------------------------------------
void toolModel::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
	if (name == _id("..."))
	{
		
	}
}


