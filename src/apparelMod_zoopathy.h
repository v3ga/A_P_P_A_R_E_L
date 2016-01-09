//
//  apparelMode_zoopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"

class Poil
{
	public:
		Poil				();
	
		void				draw();


		ofPolyline			m_line;
		int					m_indexFace;
};



class apparelMod_zoopathy : public apparelMod
{
	public:
		apparelMod_zoopathy	();

		void					createParameters	();
		void					apply				();
		void					drawExtra			();
		void					deletePoils			();

	private:
	
		vector<Poil*>			m_poils;
		int						m_nbPoints;
 
		ofParameter<float>		m_lengthMax;
		ofParameter<float>		m_density;
		ofParameter<float>		m_resolution;
};