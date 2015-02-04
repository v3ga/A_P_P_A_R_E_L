//
//  parameterGroupLowHigh.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 04/02/2015.
//
//

#pragma once
#include "ofMain.h"

class parameterGroupLowHigh
{
	public:
		static void		create(ofParameterGroup& group, string name, float min, float low, float high, float max);
};

