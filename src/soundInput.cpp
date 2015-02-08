//
//  soundInput.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 08/02/2015.
//
//

#include "soundInput.h"
#include "globals.h"

//--------------------------------------------------------------
SoundInput::SoundInput()
{
	m_soundBufferSize = 0;
	m_volMaxInput = 0;
}

//--------------------------------------------------------------
SoundInput::~SoundInput()
{
}

//--------------------------------------------------------------
void SoundInput::setup(int deviceId, int nChannels)
{
	m_soundStreamInput.listDevices();
	if (deviceId>=0)
	{
		m_soundBufferSize = 256;

		setupAudioBuffers(nChannels);

		m_soundStreamInput.setDeviceID(deviceId);
		m_soundStreamInput.setup(ofGetAppPtr(), 0, nChannels, 44100, m_soundBufferSize, 4);
		m_soundStreamInput.start();

	}
}


//--------------------------------------------------------------
void SoundInput::stop()
{
	m_soundStreamInput.stop();
}

//--------------------------------------------------------------
void SoundInput::setupAudioBuffers(int nbChannels)
{
    if (nbChannels == 1)
    {
        m_soundMono.assign(m_soundBufferSize, 0.0f);
    }
    else
    if (nbChannels == 2)
    {
        m_soundLeft.assign(m_soundBufferSize, 0.0f);
        m_soundRight.assign(m_soundBufferSize, 0.0f);
    }
}

//--------------------------------------------------------------
void SoundInput::saveParameters()
{
	
}

//--------------------------------------------------------------
void SoundInput::loadParameters()
{

}

//--------------------------------------------------------------
void SoundInput::audioIn(float * input, int bufferSize, int nChannels)
{
	float curVol = 0.0;
    int numCounted = 0;

    if (nChannels==1)
    {
		for (int i = 0; i < bufferSize; i++){
			m_soundMono[i] = input[i];
			curVol += m_soundMono[i]*m_soundMono[i];
			numCounted++;
		}

        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
	 
        // this is how we get the root of rms :)
        curVol = sqrt( curVol );

		curVol = ofMap(curVol, 0.0f,m_volMaxInput, 0.0f,1.0f);

		GLOBALS->setSoundInputVolume( curVol );
    }
    else
    if (nChannels==2)
    {
        // samples are "interleaved"
        
        for (int i = 0; i < bufferSize; i++)
        {
            m_soundLeft[i]	= input[i*2]*0.5;
            m_soundRight[i]	= input[i*2+1]*0.5;
            
            curVol += m_soundLeft[i] * m_soundLeft[i];
            curVol += m_soundRight[i] * m_soundRight[i];
            numCounted+=2;
        }
        
        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :)
        curVol = sqrt( curVol );
        
		GLOBALS->setSoundInputVolume( curVol );
	 
        //bufferCounter++;
    }
}
