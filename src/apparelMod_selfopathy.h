//
//  apparelMod_selfopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/02/2015.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_selfopathy : public apparelMod
{
	public:
		apparelMod_selfopathy	();
		~apparelMod_selfopathy	();


/*		void					apply				();
		void					onParameterChanged	(ofAbstractParameter& parameter);
*/		void					drawFaces			();

	private:
		ofImage					m_image;

};