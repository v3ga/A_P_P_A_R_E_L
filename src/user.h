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

class userSocialInterface;
class userConfigurationInfo;
class apparelModManager;
//--------------------------------------------------------------
class user : public ofThread
{
	public:

		virtual		~user									();

					void 				setId				(string id_){m_id = id_;}
					string				getId				(){return m_id;}
					void				setModManager		(apparelModManager* p){mp_modManager=p;}
					apparelModManager*	getModManager		(){return mp_modManager;}
					string				getPath				(string filename){return "users/"+m_id+"/"+filename;}
					ofxXmlSettings&		getConfiguration	(){return m_configuration;}
					void				loadServicesData	();
					void				saveServicesData	();
	

		virtual		void				loadConfiguration	();
		virtual		void				update				(float dt);
		virtual 	void 				threadedFunction	();
					void				onNewTick			(ofxTickerEventArgs& args);
	
					void				onNewText			(string text);
	
	protected:
		string							m_id;

		// Ticker to reload data
		ofxTicker						m_ticker;

		// Configuration settings (configuration.xml contains the list of services for example)
		ofxXmlSettings					m_configuration;

		// A list of «services» used to retrieve data
		vector<userSocialInterface*>	m_listSocialInterfaces;
	
		// Mod Manager to inform that new data is available from services
		apparelModManager*				mp_modManager;
	

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

