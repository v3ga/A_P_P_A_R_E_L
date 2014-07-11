//
//  oscSender.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 09/07/2014.
//
//

#pragma once
#include "ofMain.h"


class oscSenderInterface
{
	public:
		virtual void setup			(string ip, int port){}
		virtual void sendParameter	(const ofAbstractParameter & parameter){}
};

#ifdef TARGET_OSX
#include "ofxOscSender.h"
class oscSender : public oscSenderInterface
{
	public:
		void setup			(string ip, int port);
		void sendParameter	(const ofAbstractParameter & parameter);

		ofxOscSender		m_sender;
};

#endif