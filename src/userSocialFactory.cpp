//
//  userSocialFactory.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#include "userSocialFactory.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
userSocialInterface* userSocialFactory::makeInstance( user* pUser, string name )
{
	if (name == "twitter")
	{
		#ifdef TARGET_OF_OSX
   		userTwitter* pUserTwitter = new userTwitter(pUser);
		return (userSocialInterface*) pUserTwitter;
		#endif

		#ifdef TARGET_OF_IOS
   		userTwitterGuestIOS* pUserTwitter = new userTwitterGuestIOS(pUser);
		return (userSocialInterface*) pUserTwitter;
		#endif
	}

	return 0;
}

//--------------------------------------------------------------
userSocialInterface* userSocialFactory::makeInstance(userConfigurationInfo* pConfigInfo)
{
	if (pConfigInfo == 0) return 0;

	OFAPPLOG->begin("userSocialFactory::makeInstance()");

	// TWITTER
	#ifdef TARGET_OF_OSX
	if (pConfigInfo->m_name == "twitter")
	{
   		OFAPPLOG->println("- creating service \""+pConfigInfo->m_name+"\"");
   		userTwitter* pUserTwitter = new userTwitter(pConfigInfo->mp_user);
   		if (pUserTwitter->setup(pConfigInfo->mp_configuration, pConfigInfo->m_configTagIndex))
   		{
			return (userSocialInterface*) pUserTwitter;
   		}
   		else
   		{
	   		delete pUserTwitter;
   		}
	}
	#endif
	
	// TWITTER IOS
	#ifdef TARGET_OF_IOS
	if (pConfigInfo->m_name == "twitterGuestIOS")
	{
   		OFAPPLOG->println("- creating service \""+pConfigInfo->m_name+"\"");
   		userTwitterGuestIOS* pUserTwitter = new userTwitterGuestIOS(pConfigInfo->mp_user);
   		if (pUserTwitter->setup(pConfigInfo->mp_configuration, pConfigInfo->m_configTagIndex))
   		{
			return (userSocialInterface*) pUserTwitter;
   		}
   		else
   		{
	   		delete pUserTwitter;
   		}
	}
 	#endif
 
 
   OFAPPLOG->end();

   return 0;
}
