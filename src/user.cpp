//
//  user.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 16/07/2014.
//
//

#include "user.h"
#include "userTwitter.h"
#include "ofAppLog.h"

user::~user()
{
	// TODO : delete services
}


void user::loadConfiguration()
{
	OFAPPLOG->begin("user::loadConfiguration()");

	string pathFile = getPath("configuration.xml");
	OFAPPLOG->println("- loading file " + pathFile);

	if (m_configuration.load(pathFile))
	{
		OFAPPLOG->println("- loaded file");

		// Services factory
		int nbServices = m_configuration.getNumTags("user:services");
		OFAPPLOG->println("- found "+ofToString(nbServices)+" service(s)");
	
		for (int i=0; i<nbServices; i++)
		{
			string serviceName = m_configuration.getAttribute("user:service", "name", "", i);
			if (serviceName != ""){

				// TWITTER
				if (serviceName == "twitter")
				{
					OFAPPLOG->println("- creating service \""+serviceName+"\"");
					userTwitter* pUserTwitter = new userTwitter(this);
					if (pUserTwitter->setup(m_configuration, i))
					{
						m_listSocialInterfaces.push_back( pUserTwitter );
					}
					else
					{
						delete pUserTwitter;
					}
				}

			}
		}

	}
	else
	{
		OFAPPLOG->println(OF_LOG_ERROR, "-error loading file");
	}
	
	OFAPPLOG->end();
}

void user::update()
{
	vector<userSocialInterface*>::iterator it;
	for (it = m_listSocialInterfaces.begin() ; it != m_listSocialInterfaces.end(); ++it){
		(*it)->update();
	}
	
}



