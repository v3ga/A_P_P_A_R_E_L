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

class PorcuVertex
{
	public:
		PorcuVertex			(ofVec3f p, ofVec3f n, int i){
			m_pos 		= p;
			m_normal 	= n;
			m_index		= i;
		}
	
		ofVec3f				m_pos;
		ofVec3f				m_normal;
		int					m_index; // in mesh
};

class apparelMood_porcupinopathy : public apparelMod
{
	public:
		apparelMood_porcupinopathy	();
		~apparelMood_porcupinopathy	();


		void				createParameters	();
		void				apply				();
		void				update				();
		void				onParameterChanged	(ofAbstractParameter& parameter);

	private:
		void				deleteVerticesPorcu();
 
 
 		ofMesh				m_meshSpikes;
		vector<PorcuVertex*>m_verticesPorcu;
 
		ofParameter<float>	m_amplitude;
};