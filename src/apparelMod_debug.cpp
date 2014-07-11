//
//  apparelMod_debug.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/06/2014.
//
//

#include "apparelMod_debug.h"
#include "apparelModel.h"

apparelMod_debug::apparelMod_debug() : apparelMod("Debug")
{
	m_parameter.set("param", 0.5f, 0.0f, 1.0f);
	m_parameters.add(m_parameter);
}

void apparelMod_debug::loadParameters()
{
	apparelMod::loadParameters();
//	vector<ofMeshFace>& modelFaces = mp_model->getMeshFacesRef();
}
