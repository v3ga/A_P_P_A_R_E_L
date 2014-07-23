//
//  toolUser.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 23/07/2014.
//
//

#include "toolUser.h"
#include "user.h"
#include "userSocialInterface.h"

//--------------------------------------------------------------
toolUser::toolUser(toolManager* parent, user* pUser) : tool("User", parent)
{
	mp_user 		= pUser;
	mp_lblTitle 	= 0;
}

//--------------------------------------------------------------
void toolUser::createControlsCustom()
{
	if (mp_canvas==0) return;

	mp_lblTitle = mp_canvas->addLabel("User:" + (mp_user ? mp_user->getId() : "???"));
	mp_canvas->addSpacer(300,1);


	if (mp_user)
	{
	  vector<string> servicesIds;

	  vector<userSocialInterface*>::iterator it;
	  vector<userSocialInterface*>& userServices = mp_user->getListServices();

	  for (it = userServices.begin(); it != userServices.end(); ++it)
	  {
		  servicesIds.push_back( (*it)->getId() );
	  }

		mp_canvas->addRadio("Services", servicesIds);


	 }


	mp_canvas->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void toolUser::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
}




