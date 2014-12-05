//
//  apparelMode_porcupinopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#pragma once


#include "apparelMod.h"

class apparelMode_porcupinopathy : public apparelMod
{
	public:
		apparelMode_porcupinopathy	();


		void				apply				();
		void				update				();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter){}

	private:
		vector<ofVec3f>		m_offsets;
		ofParameter<float>	m_amplitude;
};