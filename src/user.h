//
//  user.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxTicker.h"
#include "ofxSQLite.h"

class userSocialInterface;
class userConfigurationInfo;
class apparelModManager;
//--------------------------------------------------------------
class user : public ofThread
{
	public:
					user									();
		virtual		~user									();
 
					void 				setId				(string id_){m_id = id_;}
					string				getId				(){return m_id;}
					void				setTemplate			(bool is=true){m_bTemplate = is;}
					bool				getTemplate			(){return m_bTemplate;}
					void				setModManager		(apparelModManager* p){mp_modManager=p;}
					apparelModManager*	getModManager		(){return mp_modManager;}
					string				getPathRelative		(string filename="");
					string				getPathRelativeForUserId(string userId,string filename="");
					string				getPathDocument		(string filename="");
					string				getPathDocumentForUserId(string userId,string filename="");
					string				getPathResources	(string filename="");
					string				getPathResourcesForUserId	(string userId, string filename="");
 
					ofxXmlSettings&		getConfiguration	(){return m_configuration;}
					void				loadServicesData	();
					void				saveServicesData	();
					bool				areServicesSetup	();

					void				useThread			(bool is=true){m_bUseThread=is;}
 					void				useTick				(bool is=true){m_bUseTick = is;}		// should be called before loadconfiguration
	
					void				deconnect			();
					void				connect				();
					bool				isConnected			(){return m_bConnected;}
 
					void				createDirectory		();
					void				createDocumentDirectory();
					void				createFileDataSql	();

					vector<userSocialInterface*>&	getListServices		(){return m_listSocialInterfaces;}
					userSocialInterface*			getService			(string id);
 


		virtual		void				loadConfiguration	();
					string				getPathSqlData		();
					void				connectSqlData		();
					ofxSQLite*			getSqlData			(){return mp_sqlData;}
		virtual		void				update				(float dt);
		virtual 	void 				threadedFunction	();
		virtual		float				getPeriodTick		(){return m_periodTick;}
					void				onNewTick			(ofxTickerEventArgs& args);
	
					// Called asynchronously in the thread (doWork method of userSocialInterface)
					void				onNewText			(string text);
					void				onNewWords			(vector<string>& words);
	
	protected:
		// zeroAll
		void							zeroAll				();

		// Id
		string							m_id;

		// Template
		bool							m_bTemplate;

		// Connected
		bool							m_bConnected;

		// Thread params
		bool							m_bUseThread;		// some services may use their own thread (ex : Fabric / Twitter), used to call service->doWork()


 
		// Ticker to reload data
		ofxTicker						m_ticker;
		float							m_periodTick;
		bool							m_newTick;			// used in update method
		bool							m_bUseTick;			// user cannot do ticking (example for template)

		// Configuration settings (configuration.xml contains the list of services for example)
		ofxXmlSettings					m_configuration;

		// A list of «services» used to retrieve data
		vector<userSocialInterface*>	m_listSocialInterfaces;
	
		// Mod Manager to inform that new data is available from services
		apparelModManager*				mp_modManager;
	
		// Temp
		ofxSQLite*						mp_sqlData;
 

};

//--------------------------------------------------------------
class userConfigurationInfo
{
	public:
		userConfigurationInfo							(user* pUser, string configTagName, int configTagIndex)
		{
			mp_user 		= pUser;
			m_configTagName = configTagName;
			m_configTagIndex= configTagIndex;
			
			mp_configuration= &mp_user->getConfiguration();
			m_name 			= mp_configuration->getAttribute(m_configTagName, "name", "", configTagIndex);
		}


		// User parent
		user*						mp_user;

		// Data in the configuration.xml file of user
		ofxXmlSettings*				mp_configuration;
		string						m_configTagName;
		int							m_configTagIndex;

		// Name of this configuration (attribute «name» of m_configTagName passed as arguments)
		string						m_name;
};

