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
		oscSenderInterface			()
		{
			m_isSetup = false;
		}

		virtual void setup			(string ip, int port){}
		virtual void sendParameter	(const ofAbstractParameter & parameter){}
		virtual void setActive		(bool is=true){m_isActive=is;}
				bool isSetup		(){return m_isSetup;}
				bool isActive		(){return m_isActive;}
	protected:
		bool		m_isSetup;
		bool		m_isActive;

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