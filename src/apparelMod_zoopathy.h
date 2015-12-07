//
//  apparelMode_zoopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_zoopathy : public apparelMod
{
	public:
		apparelMod_zoopathy	();

		void					drawExtra			();

	private:
	
		map<int, ofPolyline*>	m_mapPolylines;
		int						m_nbPoints;

};