//
//  toolNetwork.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#include "toolNetwork.h"
#include "globals.h"
#include "oscSender.h"

//--------------------------------------------------------------
toolNetwork::toolNetwork(toolManager* parent) : tool("Network", parent)
{
	mp_lblOscSending 	= 0;
	mp_oscSender		= 0;
}

//--------------------------------------------------------------
void toolNetwork::createControlsCustom()
{
	if (mp_canvas==0) return;
	
	mp_canvas->addLabel("Network");
	mp_canvas->addSpacer(300,1);


	mp_oscSender = GLOBALS->getOscSender();
	if (mp_oscSender)
	{
		mp_canvas->addToggle("Activate OSC", true);
		mp_lblOscSending = mp_canvas->addLabel( "Sending to remote "+mp_oscSender->getIP()+":"+ofToString(mp_oscSender->getPort()), OFX_UI_FONT_SMALL );

//		ofxUILabelButton* pbtnEdit = new ofxUILabelButton("edit", true, 0, mp_lblOscSending->getRect()->getHeight(),0,0,OFX_UI_FONT_SMALL);
//		mp_canvas->addWidgetRight( pbtnEdit );

	}

	mp_canvas->autoSizeToFitWidgets();

	updateUI();
}

//--------------------------------------------------------------
void toolNetwork::updateUI()
{
	if (mp_lblOscSending && mp_oscSender)
		mp_lblOscSending->setColorFill(ofxUIColor(255,mp_oscSender->isActive() ? 255 : 80));
}

//--------------------------------------------------------------
void toolNetwork::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Activate OSC")
	{
		if (mp_oscSender)
			mp_oscSender->setActive( ((ofxUIToggle*) e.widget)->getValue() );

		updateUI();
		
	}
}

