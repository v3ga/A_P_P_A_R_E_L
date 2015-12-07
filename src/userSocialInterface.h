//
//  userSocialInterface.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#pragma once
#include "user.h"
#include "ofxXmlSettings.h"

class userSocialInterface
{
	public:
		userSocialInterface							(string id, user* p);

		virtual		bool		setup				(ofxXmlSettings* pConfig, int serviceIndex){return false;}
		virtual		void		update				(float dt){}
		virtual		void		doWork				(){}
		virtual		void		analyzeData			(){}
		virtual 	void		loadData			(){}
		virtual		void		saveData			(){}

					user*		getUser				(){return mp_user;}
					string		getId				(){return m_id;}

		bool		m_bSetup;

   protected:
		string		m_id;

		user*		mp_user;
		bool		m_isRunOnce;
 
};