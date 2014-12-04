//
//  apparelMod_sportopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_sportopathy : public apparelMod
{
	public:
		apparelMod_sportopathy	();

 
 		void	apply				();
		void	onParameterChanged	(ofAbstractParameter& parameter);

};
