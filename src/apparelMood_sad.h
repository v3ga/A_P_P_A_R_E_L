//
//  apparelMood_sad.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"

class apparelMood_sad;
class Drop
{
	public:
		Drop				(apparelMood_sad*, ofVec3f p);
 
		void				update	(float dt);
		void				draw	();
	
		apparelMood_sad*	mp_parent;
		ofVec3f				m_posEmit;
		ofVec3f				m_posA,m_posB;
 		float				m_speed;
		float				m_length;
		bool				m_bFollow;
 
};

class apparelMood_sad : public apparelMod
{
	public:
		apparelMood_sad	();


		void				createParameters	();
		void				apply				();
		void				update				();
		void				drawExtra			();
		void				copyModelFrom		(const apparelModel& model);
		void				onParameterChanged	(ofAbstractParameter& parameter);
 

		ofParameter<float>	m_zEmitDrop;
		ofParameter<float>	m_zRemoveDrop;
		ofParameter<float>	m_zSpeedDrop;
		ofParameter<float>	m_lengthDrop;


		vector<Drop*>		m_drops;
		void				deleteDrops			();
};
