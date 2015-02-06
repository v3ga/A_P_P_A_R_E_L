//
//  apparelMode_porcupinopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#pragma once


#include "apparelMod.h"
#include "ofxButterfly.h"

class porcuVertexData
{
	public:
		porcuVertexData		(ofIndexType index, ofVec3f n, ofVec3f middle, float ampMin, float ampMax)
		{
			m_index 			= index;
			m_normal 			= n;
			m_middleFace 		= middle;
			m_amplitude			= 0.0f;
			m_amplitudeFactor	= ofRandom(ampMin,ampMax);
			m_amplitudeSpeed	= ofRandom(ampMin,ampMax)*0.1f+0.1f;
		}
 
 
		ofIndexType			m_index;
		ofVec3f				m_normal;
		ofVec3f				m_middleFace;
		float				m_amplitude, m_amplitudeFactor;
		float				m_amplitudeSpeed;
};

class apparelMood_porcupinopathy : public apparelMod
{
	public:
		apparelMood_porcupinopathy	();
		~apparelMood_porcupinopathy	();

		void				apply				();
		void				update				();
		void				onParameterChanged	(ofAbstractParameter& parameter);

	private:
	    ofxButterfly 				butterfly;
 
		ofParameter<float>			m_amplitude;
		ofParameter<float>			m_levelSubdiv;
		vector<porcuVertexData*>	m_extrusionData;
 		ofParameterGroup			m_amplitudeRndFactor;

 
 		void						deleteExtrusionData();
 
};