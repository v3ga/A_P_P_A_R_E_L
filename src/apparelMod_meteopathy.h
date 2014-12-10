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


		void					update				();
		void					draw				();
		void					onParameterChanged	(ofAbstractParameter& parameter);

	private:
		vector<of3dPrimitive*>	m_primitives;
		void					deletePrimitives	();
	
};