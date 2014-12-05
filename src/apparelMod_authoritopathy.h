//
//  apparelMod_authority.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#pragma once
#include "apparelMod.h"

class apparelMod_authoritopathy : public apparelMod
{
	public:
		apparelMod_authoritopathy	();


		void				apply				();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter);

	private:
		ofVec3f				m_pointInfluence;
 		ofParameter<float>	m_displacement;

};