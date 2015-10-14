//
//  apparelMood_noisopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#pragma once

#include "apparelMod.h"
#include "parameterGroupLowHigh.h"

class apparelMood_noisopathy : public apparelMod
{
	public:
		apparelMood_noisopathy	();


		void				createParameters	();
		void				apply				();
		void				update				();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter){}

	private:
		vector<ofVec3f>			m_offsets;
		ofParameter<float>		m_amplitude;
		ofParameter<float>		m_timeScale;
};
