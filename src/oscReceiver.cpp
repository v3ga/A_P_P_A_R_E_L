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


			if (m_oscMessage.getAddress() == OSC_MOD_SET_PARAMETER)
			{
				apparelModManager* pModManager = GLOBALS->getModManager();
				if (pModManager)
				{
					string instanceName = m_oscMessage.getArgAsString(0);
					string parameterName = m_oscMessage.getArgAsString(1);
					
					OFAPPLOG->println("instanceName="+instanceName+"/parameterName="+parameterName);
					
					
					apparelMod* pMod = pModManager->getMod(instanceName);
					if (pMod)
					{
						ofAbstractParameter& modParam = pMod->getParameter(parameterName);
						setParameterValue(modParam, m_oscMessage, 2);
						
						OFAPPLOG->println("value for "+instanceName+"/"+parameterName+"="+modParam.toString());
					}
				
				}
	   		}

		}
	}
}

void oscReceiver::setParameterValue(ofAbstractParameter& param, ofxOscMessage& msg, int oscArgIndex)
{
	ofAbstractParameter* p = &param;

	if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_INT32){
	   p->cast<int>() = msg.getArgAsInt32(oscArgIndex);
   }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_FLOAT){
	   p->cast<float>() = msg.getArgAsFloat(oscArgIndex);
   }else if(p->type()==typeid(ofParameter<bool>).name() && msg.getArgType(oscArgIndex)==OFXOSC_TYPE_INT32){
	   p->cast<bool>() = msg.getArgAsInt32(oscArgIndex);
   }else if(msg.getArgType(oscArgIndex)==OFXOSC_TYPE_STRING){
	   p->fromString(msg.getArgAsString(oscArgIndex));
   }

}

