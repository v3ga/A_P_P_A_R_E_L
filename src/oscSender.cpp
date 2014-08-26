//
//  oscSender.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 09/07/2014.
//
//

#include "oscSender.h"



#ifdef TARGET_OSX
#include "oscDefs.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
void oscSender::setup(string ip, int port)
{
	m_sender.setup(ip, port);
	m_isSetup = true;
	m_isActive = true;
	m_ip = ip;
	m_port = port;
}

//--------------------------------------------------------------
void oscSender::sendParameter(const ofAbstractParameter & parameter)
{
	if(!isSetup() || !isActive()) return;
	// if(!parameter.isSerializable()) return;

	string address_prefix = OSC_MOD_SET_PARAMETER;
	
	if(parameter.type()==typeid(ofParameterGroup).name())
	{
	}
	else
	{
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();

		ofxOscMessage msg;
		msg.setAddress(OSC_MOD_SET_PARAMETER);
		msg.addStringArg(hierarchy[0]); // instance name
		msg.addStringArg(hierarchy[1]); // parameter name


	  if(parameter.type()==typeid(ofParameter<int>).name()){
		  msg.addIntArg(parameter.cast<int>());
	  }else if(parameter.type()==typeid(ofParameter<float>).name()){
		  msg.addFloatArg(parameter.cast<float>());
	  }else if(parameter.type()==typeid(ofParameter<bool>).name()){
		  msg.addIntArg(parameter.cast<bool>());
	  }else{
		  msg.addStringArg(parameter.toString());
	  }

		m_sender.sendMessage(msg);


//		OFAPPLOG->println(hierarchy[0]+"/"+hierarchy[1]);
/*
*/


//		m_sender.appendParameter(msg,parameter,address);
	
/*		string address = address_prefix;
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+= "/" + hierarchy[i];
		}
		if(address.length()) address += "/";
		ofxOscMessage msg;
		m_sender.appendParameter(msg,parameter,address);
		m_sender.sendMessage(msg);
*/
	}


}



#endif