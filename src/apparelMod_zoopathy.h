//
//  apparelMode_zoopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"
#include "parameterGroupLowHigh.h"

class Poil
{
	public:
		Poil				(int res, float rnd);
	
		void				draw();

		int					m_resolution;
		ofVec3f				m_origin;
		ofVec3f				m_normal;
		ofPolyline			m_line;
		int					m_indexFace;
		vector<ofVec3f>		m_random;

 
};



class apparelMod_zoopathy : public apparelMod
{
	public:
		apparelMod_zoopathy	();

		void					createParameters	();
		void					apply				();
		void					update				();
		void					drawExtra			();
		void					deletePoils			();
		void					onParameterChanged	(ofAbstractParameter& parameter);
		void					onWeightChanged		();

		vector<Poil*>			m_poils;
		bool					m_bDoUpdatePoils;

	private:
		int						m_nbFaceSelfo;
		bool					m_bDoCreatePoils;
	
		int						m_nbPoints;
 
		ofParameter<float>		m_lengthMax;
		ofParameter<float>		m_density;
		ofParameter<int>		m_resolution;
		ofParameter<float>		m_random;
		ofParameterGroup		m_zPoils;
};