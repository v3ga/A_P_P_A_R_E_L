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
		void				loadModel				();
		void				saveModel				();
		void				onParameterChanged		(ofAbstractParameter& parameter);

	private:
 		ofParameter<float>	m_displacement;
 
 		ofParameter<float>	m_dispAxisX;
 		ofParameter<float>	m_dispAxisY;
 		ofParameter<float>	m_dispAxisZ;
 
 
		map<int, ofVec3f>	m_dispVertex;
 
};
