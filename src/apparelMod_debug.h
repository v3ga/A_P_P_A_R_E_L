//
//  apparelMod_debug.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/06/2014.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_debug : public apparelMod
{
	public:
		apparelMod_debug	();

		void				loadParameters	();

		ofMesh					m_submesh;
		ofParameter<float>		m_parameter;
};
