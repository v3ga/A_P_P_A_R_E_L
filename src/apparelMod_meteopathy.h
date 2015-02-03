//
//  apparelMod_meteopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 06/12/2014.
//
//

#pragma once
#include "apparelMod.h"

class apparelMod_meteopathy : public apparelMod
{
	public:
		apparelMod_meteopathy	();
		~apparelMod_meteopathy	();


		void					apply				();
		void					onParameterChanged	(ofAbstractParameter& parameter);

	private:
		vector<ofIcoSpherePrimitive*> m_particles;

		ofParameter<float>		m_positionXMin,m_positionXMax;
		ofParameter<float>		m_positionYMin,m_positionYMax;
		ofParameter<float>		m_positionZMin,m_positionZMax;
		ofParameter<float>		m_sizeMin,m_sizeMax;
 
		int						m_nbParticles;
		void					deleteParticles();

};