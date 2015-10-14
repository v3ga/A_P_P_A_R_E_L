//
//  apparelMod_kawaiopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#pragma once

#include "apparelMod.h"

class apparelMod_kawaiopathy;
class kawai
{
	public:
		kawai					(apparelMod_kawaiopathy*);
	
		virtual bool			readXml		(ofxXmlSettings& settings, int which);
		virtual void			loadData	();
	
		apparelMod_kawaiopathy*	mp_parent;
 
		ofxAssimpModelLoader	m_model;
		ofMesh					m_mesh;
 
		string					m_meshName;
 		float					m_scale;
		ofVec3f					m_pos;
		ofQuaternion			m_rot;
		float					m_weight;
};

class apparelMod_kawaiopathy : public apparelMod
{
	public:
		apparelMod_kawaiopathy	();
		~apparelMod_kawaiopathy	();


		void					apply				();
/*		void					onParameterChanged	(ofAbstractParameter& parameter);
*/
		void					readModel			();
//		void					drawFaces			();
		void					drawExtra			();

	private:
		ofxAssimpModelLoader 	models[4];
		vector<kawai*>			m_kawais;

};