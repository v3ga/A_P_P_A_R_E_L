//
//  soundInput.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 08/02/2015.
//
//

#pragma once
#include "ofMain.h"

class SoundInput
{
    public:
 
	   	 SoundInput							();
		~SoundInput							();
	
        virtual void        setup           (int deviceId, int nChannels);
        virtual void        audioIn         (float * input, int bufferSize, int nChannels);
		virtual void 		stop			();
				void		setupAudioBuffers(int nChannels);

				void		saveParameters	();
				void		loadParameters	();

		ofSoundStream		m_soundStreamInput;
		int                 m_soundBufferSize;
		vector<float>		m_soundMono,m_soundLeft,m_soundRight;
 
 		float				m_volMaxInput;
};

