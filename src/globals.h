//
//  globals.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#pragma once
#include "ofAppLog.h"

class ofApp;
class oscSenderInterface;
#define GLOBALS	globals::getInstance()

class globals
{
	public:
				globals			();
	
		static 	globals*		getInstance();

				void			setApp(ofApp* p){pOfApp = p;}
				ofApp*			getApp(){return pOfApp;}

				void				setOscSender(oscSenderInterface* p){pOscSender = p;}
				oscSenderInterface*	getOscSender(){return pOscSender;}
	
	private:
		static 	globals*				instance;
				ofApp*					pOfApp;
				oscSenderInterface*		pOscSender;
};
