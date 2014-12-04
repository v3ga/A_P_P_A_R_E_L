//
//  apparelMood_noisopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#pragma once

#include "apparelMod.h"

class apparelMood_noisopathy : public apparelMod
{
	public:
		apparelMood_noisopathy	();


		void				apply				();
		void				update				();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter){}

	private:
		ofMesh				m_meshOriginal;
		vector<ofVec3f>		m_offsets;
};
