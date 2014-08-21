//
//  user.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#include "user.h"
#include "userSocialFactory.h"
#include "apparelModMAnager.h"
#include "ofAppLog.h"


//--------------------------------------------------------------
user::user()
{
	m_periodTick	=	15.0f;
}

//--------------------------------------------------------------
user::~user()
{
	// Delete services
	lock();
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end() ; ++it)
		delete *it;
	m_listSocialInterfaces.clear();
	unlock();

	// Stop ticking
	ofRemoveListener(m_ticker.newTickEvent, this, &user::onNewTick);

	// stop thread
	stopThread();
}

//--------------------------------------------------------------
void user::loadServicesData()
{
	OFAPPLOG->begin("user::saveServicesData()");
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin(); it != m_listSocialInterfaces.end(); ++it)
	{
		(*it)->loadData();
	}
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void user::saveServicesData()
{
	OFAPPLOG->begin("user::saveServicesData()");
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin(); it != m_listSocialInterfaces.end(); ++it)
	{
		(*it)->saveData();
	}
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void user::loadConfiguration()
{
	OFAPPLOG->begin("user::loadConfiguration()");

	string pathFile = getPath("configuration.xml");
	OFAPPLOG->println("- loading file " + pathFile);

	if (m_configuration.load(pathFile))
	{
		OFAPPLOG->println("- loaded file");


		// Services factory
		int nbServices = m_configuration.getNumTags("user:services:service");
		OFAPPLOG->println("- found "+ofToString(nbServices)+" service(s)");
	
		for (int i=0; i<nbServices; i++)
		{
			// Info into the xml configuration file
			userConfigurationInfo* pUserConfigInfo = new userConfigurationInfo(this, "user:services:service", i);

			// Create instance with factory
			userSocialInterface* pSocialInterface = userSocialFactory::makeInstance(pUserConfigInfo);

			// Push in list
			if (pSocialInterface){
				m_listSocialInterfaces.push_back(pSocialInterface);
			}
		
			// Delete info
			delete pUserConfigInfo;
		}

		// Services data
		loadServicesData();

		// Starting retrieving
		m_periodTick = m_configuration.getValue("user:period", 15.0f);
		OFAPPLOG->println("- period tick="+ofToString(m_periodTick)+" seconds");

		ofAddListener(m_ticker.newTickEvent, this, &user::onNewTick);
		m_ticker.setPeriod( getPeriodTick() );
		m_ticker.play();
	}
	else
	{
		OFAPPLOG->println(OF_LOG_ERROR, "-error loading file");
	}
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void user::update(float dt)
{
/*	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end(); ++it){
		(*it)->update();
	}
*/
}

//--------------------------------------------------------------
void  user::threadedFunction()
{
	lock();
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end(); ++it){
		(*it)->doWork();
	}
	unlock();
}

//--------------------------------------------------------------
void user::onNewTick(ofxTickerEventArgs& args)
{
	if (this->isThreadRunning()) return;
	
	startThread();
}

//--------------------------------------------------------------
void user::onNewText(string text)
{
	if (mp_modManager)
		mp_modManager->onNewText(text);
}

//--------------------------------------------------------------
void user::onNewWords(vector<string>& words)
{
	if (mp_modManager)
		mp_modManager->onNewWords(words);
}






