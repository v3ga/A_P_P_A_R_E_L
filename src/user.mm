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
#ifdef TARGET_OF_IOS
#include "ofxIOSExtras.h"
#endif


//--------------------------------------------------------------
user::user()
{
	m_periodTick	=	15.0f;
	mp_sqlData		= 	0;
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
string user::getPathRelative(string filename)
{
	return "users/"+m_id+"/"+filename;
}

//--------------------------------------------------------------
string user::getPathDocument(string filename)
{
	#ifdef TARGET_OF_IOS
		return ofxiOSGetDocumentsDirectory() + getPathRelative(filename);
	#else
		return ofToDataPath(getPathRelative(filename));
	#endif
}


//--------------------------------------------------------------
string user::getPathResources(string filename)
{
	return ofToDataPath(getPathRelative(filename));
}

//--------------------------------------------------------------
void user::createDirectory()
{
	OFAPPLOG->begin("user::createDirectory()");

	// Path for user
	string pathUser = getPathDocument();
	OFAPPLOG->println("- pathUser="+pathUser);
	ofDirectory dirUser(pathUser);
	if (!dirUser.exists())
	{
		OFAPPLOG->println("- creating it");
		dirUser.create(true);
	}

	// SQL Data
	string pathSqlResource = getPathResources("data.sql");
	string pathSqlDocument = getPathDocument("data.sql");

	ofFile fSqlResource(pathSqlResource);
	if (fSqlResource.exists()){
		ofFile fSqlDocument(pathSqlDocument);
		
		if (!ofFile::doesFileExist(pathSqlDocument,false))
		{
			if (ofFile::copyFromTo(pathSqlResource, pathSqlDocument, false, false))
			{
				OFAPPLOG->begin("- copied '"+pathSqlResource+ "' to '"+pathSqlDocument+"'");
			}
		}
	}

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void user::loadServicesData()
{
	OFAPPLOG->begin("user::loadServicesData()");
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

	string pathFile = getPathResources("configuration.xml");
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
		
		// Connect to db
		connectSqlData();

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
string user::getPathSqlData()
{
	return getPathDocument("data.sql");
}

//--------------------------------------------------------------
void user::connectSqlData()
{
		string pathSqlData = getPathSqlData();
		OFAPPLOG->begin("user::connectSqlData()");
		OFAPPLOG->println("- connecting to "+pathSqlData);

		mp_sqlData = new ofxSQLite();
		if (mp_sqlData->setup(pathSqlData))
		{
			OFAPPLOG->println("- OK connected to "+pathSqlData);
		}
		else
		{
			OFAPPLOG->println("- FAILED to connect to "+pathSqlData);
		
			delete mp_sqlData;
			mp_sqlData=0;
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
		mp_modManager->onNewText(this,text);
}

//--------------------------------------------------------------
void user::onNewWords(vector<string>& words)
{
	if (mp_modManager)
		mp_modManager->onNewWords(this,words);
}






