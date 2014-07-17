//
//  userTwitter.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#include "userTwitter.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
userTwitter::userTwitter(user* pUser) : userSocialInterface(pUser){

}


//--------------------------------------------------------------
bool userTwitter::setup(ofxXmlSettings* pConfig, int serviceIndex)
{
	OFAPPLOG->begin("userTwitter::setup");

	string consumer_key = pConfig->getValue("user:service:consumer_key", "", serviceIndex);
	string consumer_secret = pConfig->getValue("user:service:consumer_secret", "", serviceIndex);

	OFAPPLOG->println("- consumer_key="+consumer_key);
	OFAPPLOG->println("- consumer_secret="+consumer_secret);

	m_twitterClient.setCredentialsPathname( mp_user->getPath("credentials.xml") );
    m_twitterClient.authorize(consumer_key, consumer_secret);

	startThread();

	OFAPPLOG->end();
	return true;
}

//--------------------------------------------------------------
void userTwitter::threadedFunction()
{
	ofxTwitterSearch search;
	search.screen_name = mp_user->getId();
	search.count = 1;
	
	OFAPPLOG->println("userTwitter::threadedFunction()");
	m_twitterClient.getStatusHomeTimeline( search );
}


//--------------------------------------------------------------
void userTwitter::update()
{
	if (isThreadRunning()) return;

    if(m_twitterClient.getTotalLoadedTweets() > 0)
	{
	}
}
