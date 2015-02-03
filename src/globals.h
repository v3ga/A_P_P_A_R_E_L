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
class oscSender;
class apparelModManager;

#define GLOBALS			globals::getInstance()
#define OSC_SENDER		GLOBALS->getOscSender()

class globals
{
	public:
				globals			();
	
		static 	globals*			getInstance();

				void				setApp(ofApp* p){pOfApp = p;}
				ofApp*				getApp(){return pOfApp;}

				void				setOscSender(oscSender* p){pOscSender = p;}
				oscSender*			getOscSender(){return pOscSender;}

				void				setModManager(apparelModManager* p){pApparelModManager = p;}
				apparelModManager*	getModManager(){return pApparelModManager;}

				void				setSoundInputVolume(float v){m_soundInputVolume = v;}
				float				getSoundInputVolume(){return m_soundInputVolume;}
	
	private:
		static 	globals*				instance;
				ofApp*					pOfApp;
				oscSender*				pOscSender;
				apparelModManager*		pApparelModManager;

				float					m_soundInputVolume;

};
