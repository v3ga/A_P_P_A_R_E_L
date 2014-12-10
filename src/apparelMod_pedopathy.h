//
//  apparelMod_pedopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_pedopathy : public apparelMod
{
	public:
		apparelMod_pedopathy	();
		void				onParameterChanged		(ofAbstractParameter& parameter);

	private:
		ofVec3f				m_pointInfluence;
 		ofParameter<float>	m_displacement;
};
