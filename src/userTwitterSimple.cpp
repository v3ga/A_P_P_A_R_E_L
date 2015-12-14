//
//  userTwitterSimple.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 14/12/2015.
//
//

#include "userTwitterSimple.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
userTwitterSimple::userTwitterSimple(user* pUser) : userSocialInterface("twitter", pUser)
{
}

//--------------------------------------------------------------
void userTwitterSimple::loadData()
{
	OFAPPLOG->begin("userTwitterSimple::loadData()");

   string pathTwitter = mp_user->getPathResources("twitter.xml");
   ofxXmlSettings twitter;
   if (twitter.load( pathTwitter ))
   {
	   OFAPPLOG->println("- ok loaded "+pathTwitter);
	   
	   m_nbFollowers = twitter.getValue("followers", 0);
	   m_nbFollowing = twitter.getValue("following", 0);

	   OFAPPLOG->println("- followers="+ofToString(m_nbFollowers)+" / following="+ofToString(m_nbFollowing));

	   m_image = twitter.getValue("image", "images/twitterImageDefault.png");

	   OFAPPLOG->println("- image="+m_image);

	}
	OFAPPLOG->end();
}
