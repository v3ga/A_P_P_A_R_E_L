//
//  apparelModelModifier.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 24/10/2014.
//
//

#pragma once
#include "ofMain.h"

class apparelModel;
class apparelModelModifier
{
		typedef void (*applyCallback)();

	public:
							apparelModelModifier();
							~apparelModelModifier();

		void				apply();

	
		apparelModel*		mp_model;
	   	vector<int>			m_indicesFaces;
		vector<int>			m_indicesVertex;

		bool				m_isChanged;

		applyCallback		mpf_applyCallback;

};