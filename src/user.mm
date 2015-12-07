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
	zeroAll();
}

//--------------------------------------------------------------
user::~user()
{
	deconnect();
}

//--------------------------------------------------------------
void user::zeroAll()
{
	m_periodTick	= 15.0f;
	mp_sqlData		= 0;
	m_newTick		= false;
	m_bUseThread	= false;
	m_bUseTick		= true;
	m_bConnected	= false;
	m_bTemplate		= false;

}

//--------------------------------------------------------------
void user::deconnect()
{
	if (isConnected())
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
		if (m_bUseThread)
			stopThread();
		
		m_bConnected = false;
	}
	
	// Zero
	zeroAll();
}


//--------------------------------------------------------------
string user::getPathRelative(string filename)
{
	return getPathRelativeForUserId(m_id, filename);
}

//--------------------------------------------------------------
string user::getPathRelativeForUserId(string userId, string filename)
{
	return "users/"+userId+"/"+filename;
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
string user::getPathDocumentForUserId(string userId, string filename)
{
	#ifdef TARGET_OF_IOS
		return ofxiOSGetDocumentsDirectory() + getPathRelativeForUserId(userId, filename);
	#else
		return ofToDataPath(getPathRelativeForUserId(userId, filename));
	#endif
}


//--------------------------------------------------------------
string user::getPathResources(string filename)
{
	return ofToDataPath(getPathRelative(filename));
}


//--------------------------------------------------------------
string user::getPathResourcesForUserId(string userId, string filename)
{
	return ofToDataPath(getPathRelativeForUserId(userId, filename));
}

//--------------------------------------------------------------
userSocialInterface* user::getService(string id)
{
	int nbServices = m_listSocialInterfaces.size();
	for (int i=0;i<nbServices;i++)
	{
		if (m_listSocialInterfaces[i]->getId() == id)
			return m_listSocialInterfaces[i];
	}
	return 0;
}

//--------------------------------------------------------------
void user::createDocumentDirectory()
{
	string pathUser = getPathDocument();
	OFAPPLOG->println("- pathUser="+pathUser);
	ofDirectory dirUser(pathUser);
	if (!dirUser.exists())
	{
		OFAPPLOG->println("- creating it");
		dirUser.create(true);
	}
}

//--------------------------------------------------------------
void user::createFileDataSql()
{
	// Copy empty SQL Data (if new user, file does not exist in documents)
	string pathSqlDocument = getPathDocument("data.sql");
	OFAPPLOG->println("- pathSqlDocument="+pathSqlDocument);
	if (ofFile::doesFileExist(pathSqlDocument,false) == false)
	{
		OFAPPLOG->println("- file does not exist, copying it");
		string pathSqlRessource_empty = getPathResourcesForUserId("__empty__", "data.sql");
		OFAPPLOG->println("- pathSqlRessource_empty="+pathSqlRessource_empty);
		if (ofFile::doesFileExist(pathSqlRessource_empty,false))
		{
			if (ofFile::copyFromTo(pathSqlRessource_empty, pathSqlDocument, false, false))
			{
				OFAPPLOG->println("- OK copy done");
			}
		}

	}
}



//--------------------------------------------------------------
void user::createDirectory()
{
	OFAPPLOG->begin("user::createDirectory()");

	// Path for user
	createDocumentDirectory();
	createFileDataSql();
	
//	OFAPPLOG->println("- pathSqlResource="+pathSqlResource);
//	OFAPPLOG->println("- pathSqlDocument="+pathSqlDocument);

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
bool user::areServicesSetup()
{
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin(); it != m_listSocialInterfaces.end(); ++it)
	{
		if ((*it)->m_bSetup == false)
			return false;
	}
	return true;
}


//--------------------------------------------------------------
void user::connect()
{
   // Connect to db
   connectSqlData();


   // Starting retrieving (if not a template)
   if (m_bUseTick && !m_bTemplate)
   {
	   m_periodTick = m_configuration.getValue("user:period", 15.0f);
	   OFAPPLOG->println("- period tick="+ofToString(m_periodTick)+" seconds");

	   ofAddListener(m_ticker.newTickEvent, this, &user::onNewTick);
	   m_ticker.setPeriod( getPeriodTick() );
	   m_ticker.play();
   }
   
   m_bConnected = true;
}

//--------------------------------------------------------------
void user::loadConfiguration()
{
	OFAPPLOG->begin("user::loadConfiguration()");

	// Only twitter for now
	userSocialInterface* pSocialInterface = userSocialFactory::makeInstance(this, "twitter");
	if (pSocialInterface)
	{
		m_listSocialInterfaces.push_back(pSocialInterface);
		pSocialInterface->loadData();
	}

	if (m_bTemplate == false)
	{
	}
	else
	{
		
	}

/*
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
	}
	else
	{
		OFAPPLOG->println(OF_LOG_ERROR, "-error loading file");
	}
*/
	
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
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end(); ++it){
		(*it)->update(dt);
	}

	if (m_newTick)
	{
		m_newTick = false;

		if (!m_bUseThread && areServicesSetup())
		{
			vector<userSocialInterface*>::iterator it;
			for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end(); ++it){
				(*it)->doWork();
			}
   		}
	}
}

//--------------------------------------------------------------
void  user::threadedFunction()
{
   if (areServicesSetup()==false) return;
   
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
	m_newTick = true;
	if (m_bUseThread)
	{
		if (isThreadRunning()) return;
		startThread(true, true);
	}
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






