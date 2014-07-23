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
	if(!parameter.isSerializable()) return;

	string address_prefix = OSC_MOD_SET_PARAMETER;
	
	if(parameter.type()==typeid(ofParameterGroup).name())
	{
		string address = address_prefix+"/";
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+=hierarchy[i] + "/";
		}
		ofxOscBundle bundle;
		m_sender.appendParameter(bundle,parameter,address);
		m_sender.sendBundle(bundle);
	}
	else
	{
		string address = address_prefix;
		const vector<string> hierarchy = parameter.getGroupHierarchyNames();
		for(int i=0;i<(int)hierarchy.size()-1;i++){
			address+= "/" + hierarchy[i];
		}
		if(address.length()) address += "/";
		ofxOscMessage msg;
		m_sender.appendParameter(msg,parameter,address);
		m_sender.sendMessage(msg);
	}


}



#endif