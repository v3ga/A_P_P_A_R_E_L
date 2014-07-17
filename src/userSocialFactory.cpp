//
//  userSocialFactory.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 17/07/2014.
//
//

#include "userSocialFactory.h"
#include "ofAppLog.h"

userSocialInterface* userSocialFactory::makeInstance(userConfigurationInfo* pConfigInfo)
{
	if (pConfigInfo == 0) return 0;
 
	OFAPPLOG->begin("userSocialFactory::makeInstance()");

   // TWITTER
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
 
   OFAPPLOG->end();
}
