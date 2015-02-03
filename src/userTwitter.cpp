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
userTwitter::userTwitter(user* pUser) : userSocialInterface("Twitter", pUser)
{
	m_tweetLastId = -1;
}


//--------------------------------------------------------------
bool userTwitter::setup(ofxXmlSettings* pConfig, int serviceIndex)
{
	OFAPPLOG->begin("userTwitter::setup");

	// Extract data from global config
	string consumer_key 	= pConfig->getValue("user:services:service:consumer_key", "", serviceIndex);
	string consumer_secret 	= pConfig->getValue("user:services:service:consumer_secret", "", serviceIndex);

	OFAPPLOG->println("- consumer_key="+consumer_key);
	OFAPPLOG->println("- consumer_secret="+consumer_secret);

	m_twitterClient.setCredentialsPathname( mp_user->getPathResources("twitter/credentials.xml") );
    m_twitterClient.authorize(consumer_key, consumer_secret);

	// Load data (previous grabbing infos saved into data.xml)
	OFAPPLOG->println("- loading data");

	OFAPPLOG->end();
	return true;
}

//--------------------------------------------------------------
void userTwitter::doWork()
{
	// SEARCH PARAMETERS
	ofxTwitterSearch search;
	search.screen_name = mp_user->getId();
	search.count = 1;
	search.since_id = m_tweetLastId_str;

	// SEARCH
	m_twitterClient.getStatusHomeTimeline( search );
	int nbTweets = m_twitterClient.getTotalLoadedTweets();

	// ANALYZE
    if( nbTweets > 0)
	{
		for (int i=0;i<nbTweets;i++)
		{
			// Get tweet
    	    m_tweet = m_twitterClient.getTweetByIndex(i);

			// Last Id
			m_tweetLastId_str = m_tweet.id_str;

			// Relays to user (who will relay to mods in turn)
			if (mp_user)
			{
				// Text
				OFAPPLOG->println("- got tweet id="+m_tweetLastId_str+", text='"+m_tweet.text+"'");

				// Raw text
				mp_user->onNewText( m_tweet.text );

				// Words
				vector<string> words = ofSplitString(m_tweet.text, " ",true,true); // source, delimiter,ignoreEmpty,trim
		
				// Lower strings
				mp_user->onNewWords( words );
				
				// TEMP
/*				vector<string>::iterator it;
				for (it = words.begin() ; it != words.end(); ++it){
					printf("%s/", (*it).c_str());
				}
*/

			}

		}
	}
}

//--------------------------------------------------------------
void userTwitter::loadData()
{
	OFAPPLOG->begin("userTwitter::loadData()");

	if (mp_user)
	{
		// Create user twitter directory if it does not exist
		#ifdef TARGET_OF_IOS
			string pathDirUserTwitter = mp_user->getPathDocument("twitter");
			ofDirectory dirUserTwitter( pathDirUserTwitter );
			if (!dirUserTwitter.exists()){
				if (dirUserTwitter.create()){
					OFAPPLOG->begin("creating '"+pathDirUserTwitter+"'");
				}
			}
		#endif


		string pathData = mp_user->getPathDocument("twitter/data.xml");
		OFAPPLOG->println("-loading "+pathData);

		ofxXmlSettings data;
		if (data.load(pathData))
		{
			OFAPPLOG->println("-loaded");
			m_tweetLastId_str = data.getValue("tweetLastId", "-1");
			OFAPPLOG->println("m_tweetLastId_str="+m_tweetLastId_str);
		}else{ OFAPPLOG->println("-error loading file"); }
	}

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void userTwitter::saveData()
{
	OFAPPLOG->begin("userTwitter::saveData()");
	
	if (mp_user)
	{
		string pathData = mp_user->getPathDocument("twitter/data.xml");

		OFAPPLOG->println("-path="+pathData);
		
		ofxXmlSettings data;
		data.addTag("tweetLastId");
		data.setValue("tweetLastId", m_tweetLastId_str);
		if ( data.save(pathData) ){
			OFAPPLOG->println("-saving done");
		}else{
			OFAPPLOG->println(OF_LOG_ERROR, "-saving error");
		}
	}

	OFAPPLOG->end();
}




