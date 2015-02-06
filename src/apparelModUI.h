//
//  apparelModUI.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#pragma once

#include "apparelMod.h"
#include "ofxUI.h"

class apparelModUI
{
	public:
		apparelModUI			(apparelMod* mod);
		virtual ~apparelModUI	();
	

		void					createControls	(const ofVec2f& posCanvas);
		void					handleEvents	(ofxUIEventArgs& e);
		void					parameterChanged(ofAbstractParameter & parameter);
		ofxUICanvas*			getCanvas		(){return mp_canvas;}
		void					show			(bool is=true);
		void					hide			(){show(false);}

		apparelMod*				mp_mod;
		ofxUICanvas*			mp_canvas;
		ofxUISlider*			mp_sliderWeight;
};

