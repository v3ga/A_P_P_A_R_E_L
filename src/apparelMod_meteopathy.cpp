//
//  apparelMod_meteopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 06/12/2014.
//
//

#include "apparelMod_meteopathy.h"


//--------------------------------------------------------------
apparelMod_meteopathy::apparelMod_meteopathy() : apparelMod("Meteopathy")
{


	m_nbParticles.set	("nbParticles",	10, 2,20);
	parameterGroupLowHigh::create(m_size, "size",1.0,10.0,15.0,20.0);
	parameterGroupLowHigh::create(m_x, "x",-50.0,-25.0,25.0,50.0);
	parameterGroupLowHigh::create(m_y, "y",-50.0,-25.0,25.0,50.0);
	parameterGroupLowHigh::create(m_z, "z",0.0,0.0,100.0,150.0);

	m_parameters.add(m_nbParticles);
	m_parameters.add(m_size);
	m_parameters.add(m_x);
	m_parameters.add(m_y);
	m_parameters.add(m_z);

	// ofLog() << typeid(m_parameters["x"]).name();
}

//--------------------------------------------------------------
apparelMod_meteopathy::~apparelMod_meteopathy()
{
	deleteParticles();
}

//--------------------------------------------------------------
void apparelMod_meteopathy::deleteParticles()
{
	for (int i=0;i<m_particles.size();i++)
	{
		delete m_particles[i];
	}
	m_particles.clear();
}

//--------------------------------------------------------------
void apparelMod_meteopathy::apply()
{
	if (isChanged())
	{
	

		m_model.mesh = m_meshInput;
		deleteParticles();
		
		m_nbParticlesWeight = (int)(m_weight*(float)m_nbParticles);

		for (int i=0;i<m_nbParticlesWeight;i++)
		{
			ofMatrix4x4	mRot;
			mRot.rotate(ofRandom(360.0f), ofRandom(1.0f), ofRandom(1.0f), ofRandom(1.0f));

			ofIcoSpherePrimitive* pPrimitive = new ofIcoSpherePrimitive( m_weight * ofRandom(m_size.getFloat("low"),m_size.getFloat("high")), 0 );
			m_particles.push_back( pPrimitive );
	 
			int nbVertices = pPrimitive->getMesh().getVertices().size();
			float dx = ofRandom(m_weight*m_x.getFloat("low"),m_weight*m_x.getFloat("high"));
			float dy = ofRandom(m_weight*m_y.getFloat("low"),m_weight*m_y.getFloat("high"));
			float dz = ofRandom(m_weight*m_z.getFloat("low"),m_weight*m_z.getFloat("high"));
			for (int k=0; k<nbVertices ; k++)
			{
				ofVec3f& v = pPrimitive->getMesh().getVertices()[k];

				v = mRot*v;

				v.x += dx;
				v.y += dy;
				v.z += dz;
				
			}
			m_model.mesh.append( m_particles[i]->getMesh() );
		}
	}
}


//--------------------------------------------------------------
void apparelMod_meteopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	setChanged();
}
