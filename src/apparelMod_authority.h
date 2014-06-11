//
//  apparelMod_authority.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#pragma once
#include "apparelMod.h"

class apparelMod_authority : public apparelMod
{
	public:
		apparelMod_authority	();

		ofParameter<float>		m_level;
};