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
class user
{
	public:

		virtual		~user							();

					void 		setId				(string id_){m_id = id_;}
					string		getId				(){return m_id;}
					string		getPath				(string filename){return "users/"+m_id+"/"+filename;}

		virtual		void		loadConfiguration	();
		virtual		void		update				();
	
	
	protected:
		string							m_id;
		ofxXmlSettings					m_configuration;
		vector<userSocialInterface*>	m_listSocialInterfaces;
};
