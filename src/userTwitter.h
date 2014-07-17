//
//  userTwitter.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#pragma once

#include "userSocialInterface.h"
#include "ofxTwitter.h"

class userTwitter : public userSocialInterface
{
	public:
			userTwitter							(user*);
	
			bool			setup				(ofxXmlSettings* pConfig, int serviceIndex);
			void			update				();
			void			threadedFunction	();

	        ofxTwitter 		m_twitterClient;
};
