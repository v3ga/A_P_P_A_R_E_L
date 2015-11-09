//
//  userSocialFactory.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#pragma once
#include "userSocialInterface.h"

#ifdef TARGET_OF_OSX
#include "userTwitter.h"
#endif

#ifdef TARGET_OF_IOS
#include "userTwitterGuestIOS.h"
#endif

class user;
class userSocialFactory
{
	public:
		static userSocialInterface* 	makeInstance( userConfigurationInfo* );
		static userSocialInterface* 	makeInstance( user*, string name );
};
