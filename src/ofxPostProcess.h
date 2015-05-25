//
//  ofxPostProcess.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 28/08/2014.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxPostProcess : public ofxFXObject
{
	public:
    	ofxPostProcess					();
 
	    void 				update		();
};
