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
		userSocialInterface(user* p)
		{
			mp_user = p;
		}

		virtual		bool		setup				(ofxXmlSettings& config, int serviceIndex){return false;}
		virtual		void		update				(){}
		virtual		void		retrieveData		(){}
					user*		getUser				(){return mp_user;}


   protected:
		user*		mp_user;

};