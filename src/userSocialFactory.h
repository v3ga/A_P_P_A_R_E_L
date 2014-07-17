//
//  userSocialFactory.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#pragma once
#include "userSocialInterface.h"
#include "userTwitter.h"

class user;
class userSocialFactory
{
	public:
		static userSocialInterface* 	makeInstance( userConfigurationInfo* );
};
