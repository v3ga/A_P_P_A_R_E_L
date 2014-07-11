//
//  oscReceiver.h
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#pragma once

#include "ofxOsc.h"
#include "oscDefs.h"


class oscReceiver  : public ofxOscReceiver
{
    public:
        void                update      ();
    
	private:
        ofxOscMessage		m_oscMessage;
};

