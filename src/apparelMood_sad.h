//
//  apparelMood_sad.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"

class apparelMood_sad : public apparelMod
{
	public:
		apparelMood_sad	();


		void				apply				();
		void				update				();
		void				drawExtra			();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter){}

	private:
};
