//
//  userTwitter.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#include "userTwitter.h"
#include "ofAppLog.h"

userTwitter::userTwitter(user* pUser) : userSocialInterface(pUser){

}


bool userTwitter::setup(ofxXmlSettings& configuration, int serviceIndex)
{
	OFAPPLOG->begin("userTwitter::setup");

	string consumer_key = configuration.getValue("user:service:consumer_key", "", serviceIndex);
	string consumer_secret = configuration.getValue("user:service:consumer_secret", "", serviceIndex);

	OFAPPLOG->println("- consumer_key="+consumer_key);
	OFAPPLOG->println("- consumer_secret="+consumer_secret);

	m_twitterClient.setCredentialsPathname( mp_user->getPath("credentials.xml") );
    m_twitterClient.authorize(consumer_key, consumer_secret);
	m_twitterClient.getStatusHomeTimeline();

	OFAPPLOG->end();
	return true;
}

void userTwitter::update()
{
    if(m_twitterClient.getTotalLoadedTweets() > 0) {
	}
}
