//
//  userTwitterSimple.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 14/12/2015.
//
//

#pragma once

#include "userSocialInterface.h"
#include "ofxTwitter.h"

class userTwitterSimple : public userSocialInterface
{
	public:
			userTwitterSimple					(user*);
	
			void			loadData			();


			int				m_nbFollowers, m_nbFollowing;
			string			m_image;
};
