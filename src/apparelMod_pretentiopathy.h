//
//  apparelMod_pretentiopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_pretentiopathy : public apparelMod
{
	public:
		apparelMod_pretentiopathy	();


		void				apply				();
		void				onParameterChanged	(ofAbstractParameter& parameter);

	private:
 		ofMesh				m_head;
		ofVec3f				m_headPosition;
		ofParameter<float>	m_headScale;
//		ofParameter<float>	m_headPositionZ;
		ofParameterGroup	m_headPositionZ;

		void				setHeadPosition(float x, float y, float z);
};