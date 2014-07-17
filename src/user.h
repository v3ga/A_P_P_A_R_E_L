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


class userSocialInterface;
class userConfigurationInfo;
class user
{
	public:

		virtual		~user								();

					void 			setId				(string id_){m_id = id_;}
					string			getId				(){return m_id;}
					string			getPath				(string filename){return "users/"+m_id+"/"+filename;}
					ofxXmlSettings&	getConfiguration	(){return m_configuration;}

		virtual		void			loadConfiguration	();
		virtual		void			update				();
	
	
	protected:
		string							m_id;
		ofxXmlSettings					m_configuration;
		vector<userSocialInterface*>	m_listSocialInterfaces;
};

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

		string						m_name;

		user*						mp_user;
		string						m_configTagName;
		int							m_configTagIndex;

		ofxXmlSettings*				mp_configuration;
};

