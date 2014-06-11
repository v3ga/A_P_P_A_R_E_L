//
//  globals.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#pragma once
class ofApp;
#define GLOBALS	globals::getInstance()

class globals
{
	public:
				globals			();
	
		static 	globals*		getInstance();
				void			setApp(ofApp* p){pOfApp = p;}
				ofApp*			getApp(){return pOfApp;}
	
	
	private:
		static 	globals*		instance;
				ofApp*			pOfApp;
};
