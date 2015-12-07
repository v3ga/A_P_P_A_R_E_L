//
//  userSocialInterface.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#include "userSocialInterface.h"
#include "ofAppLog.h"


userSocialInterface::userSocialInterface(string id, user* p)
{
	m_id = id;
	mp_user = p;
	m_isRunOnce = false;
	m_bSetup = false;
}
