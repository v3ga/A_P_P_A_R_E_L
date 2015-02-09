//
//  oscReceiver.cpp
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#include "oscReceiver.h"
#include "globals.h"
#include "apparelModManager.h"
#include "apparelMod.h"

//--------------------------------------------------------------
void oscReceiver::update()
{
    if (hasWaitingMessages())
    {
		while(getNextMessage(&m_oscMessage))
		{
            // int indexArg = 0;
			ofLog() << "oscReceiver::update() - received " << m_oscMessage.getAddress();


			if (m_oscMessage.getAddress() == OSC_MOD_SET_WEIGHT)
			{
				apparelModManager* pModManager = GLOBALS->getModManager();
				if (pModManager)
				{
					// Name of the mod
					string instanceName = m_oscMessage.getArgAsString(0);

					// Get Mod instance
					apparelMod* pMod = pModManager->getMod(instanceName);

					// Exists ??
					if (pMod && pMod->m_isWeightManual)
					{
						pMod->setWeight( m_oscMessage.getArgAsFloat(1) );
					}
				}
			}
			else if (m_oscMessage.getAddress() == OSC_MOD_SET_PARAMETER)
			{
				apparelModManager* pModManager = GLOBALS->getModManager();
				if (pModManager)
				{
					// Name of the mod
					string instanceName = m_oscMessage.getArgAsString(0);

					// Get Mod instance
					apparelMod* pMod = pModManager->getMod(instanceName);

					// Exists ??
					if (pMod)
					{
					  if (m_oscMessage.getNumArgs() == 3)
					  {
						 string parameterName = m_oscMessage.getArgAsString(1);
					  
						 ofAbstractParameter& modParam = pMod->getParameter(parameterName);
						 setParameterValue(modParam, m_oscMessage, 2);
					 
						  //OFAPPLOG->println("instanceName="+instanceName+"/parameterName="+parameterName);
						 // OFAPPLOG->println("value for "+instanceName+"/"+parameterName+"="+modParam.toString());
					  }
					 
					  //
					  else if (m_oscMessage.getNumArgs() == 4)
					  {
						  string groupName = m_oscMessage.getArgAsString(1);
						  string parameterName = m_oscMessage.getArgAsString(2);
						  ofAbstractParameter& modGroupParam = pMod->getParameter(groupName);
						  if (modGroupParam.type()==typeid(ofParameterGroup).name())
						  {
							  setParameterValue(modGroupParam, m_oscMessage, 3);
						  }
					  }
					}
				}
	   		}

		}
	}
}

//--------------------------------------------------------------
void oscReceiver::setParameterValue(ofAbstractParameter& param, ofxOscMessage& msg, int oscArgIndex)
{
	ofAbstractParameter* p = &param;
   if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_INT32)
   {
	   p->cast<int>() = msg.getArgAsInt32(oscArgIndex);
   }
   else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_FLOAT)
   {
	   p->cast<float>() = msg.getArgAsFloat(oscArgIndex);
   }else if(p->type()==typeid(ofParameter<bool>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_INT32)
   {
	   p->cast<bool>() = msg.getArgAsInt32(oscArgIndex);
   }else if(msg.getArgType(oscArgIndex)==OFXOSC_TYPE_STRING)
   {
	   p->fromString(msg.getArgAsString(oscArgIndex));
   }

}

