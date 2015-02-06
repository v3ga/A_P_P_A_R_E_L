//
//  oscSender.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 09/07/2014.
//
//

#pragma once
#include "ofMain.h"
#include "oscSenderInterface.h"

class apparelMod;
class oscSender : public oscSenderInterface
{
	public:
		void setup					(string ip, int port);

		void sendParameter			(const ofAbstractParameter & parameter);
		void sendModData			(apparelMod* pMod);
		void selectMod				(apparelMod* pMod);
		void sendModEmptyUserDataSQL(apparelMod* pMod);

		void sendBeginCalibration	();
		void sendEndCalibration		();
		void saveCalibration		();
 

		void send					(ofxOscMessage m);

		ofxOscSender			m_sender;

};
