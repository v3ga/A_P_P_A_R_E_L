//
//  apparelModelModifier.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 24/10/2014.
//
//

#include "apparelModelModifier.h"
#include "apparelModel.h"


apparelModelModifier::apparelModelModifier()
{
	mpf_applyCallback 	= 0;
	mp_model 			= 0;
	m_isChanged 		= false;
}

apparelModelModifier::~apparelModelModifier()
{
	delete mp_model;
}

void apparelModelModifier::apply()
{
	if (mpf_applyCallback)
	{
		(*mpf_applyCallback)(this);
	}
}


