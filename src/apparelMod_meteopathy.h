//
//  apparelMod_meteopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 06/12/2014.
//
//

#pragma once
#include "apparelMod.h"
#include "parameterGroupLowHigh.h"

class apparelMod_meteopathy : public apparelMod
{
	public:
		apparelMod_meteopathy	();
		~apparelMod_meteopathy	();


		void					apply				();
		void					onParameterChanged	(ofAbstractParameter& parameter);

	private:
		vector<ofIcoSpherePrimitive*> 	m_particles;
		ofParameter<int>				m_nbParticles;
		ofParameterGroup				m_x;
		ofParameterGroup				m_y;
		ofParameterGroup				m_z;
 		ofParameterGroup				m_size;
		int								m_nbParticlesWeight;

		void							deleteParticles();
};