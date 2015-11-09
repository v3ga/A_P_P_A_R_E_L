//
//  apparelMood_sad.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMood_sad.h"


apparelMood_sad::apparelMood_sad() : apparelMod("Sad")
{
	m_isMood = true;
}

void apparelMood_sad::apply()
{
}

void apparelMood_sad::update()
{
}

void apparelMood_sad::drawExtra()
{
	
}


void apparelMood_sad::copyModelFrom(const apparelModel& model)
{
	apparelMod::copyModelFrom(model);
}
