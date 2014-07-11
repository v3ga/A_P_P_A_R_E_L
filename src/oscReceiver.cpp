//
//  oscReceiver.cpp
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#include "oscReceiver.h"

//--------------------------------------------------------------
void oscReceiver::update()
{
    if (hasWaitingMessages())
    {
		while(getNextMessage(&m_oscMessage))
		{
            int indexArg = 0;

        }
    }
}
