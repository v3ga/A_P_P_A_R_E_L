//
//  toolSound.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 08/02/2015.
//
//

#pragma once
#include "tool.h"
#include "soundInput.h"

class toolSound : public tool
{
	public:
		 toolSound			(toolManager*);

		 void				setSoundInput			(SoundInput* p){mp_soundInput=p;}
		 void				createControlsCustom	();

		 SoundInput*		mp_soundInput;
};
