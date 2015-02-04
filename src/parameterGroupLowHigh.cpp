//
//  parameterGroupLowHigh.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 04/02/2015.
//
//

#include "parameterGroupLowHigh.h"

void parameterGroupLowHigh::create(ofParameterGroup& group, string name_, float min_, float low_, float high_, float max_)
{

		group.setName(name_);

		ofParameter<float> min,low,high,max;

	    min.set("min",			min_);
	    low.set("low",			low_);
    	high.set("high",		high_);
    	max.set("max",			max_);
		
		group.add(min);
		group.add(low);
		group.add(high);
		group.add(max);

}
