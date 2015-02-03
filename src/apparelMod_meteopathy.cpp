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
	m_positionXMin.set	("xmin", 		0.0f, -50.0f, 	0.0f);
	m_positionXMax.set	("xmax", 		20.0f, 0.0f, 	50.0f);

	m_positionYMin.set	("ymin", 		0.0f, -50.0f, 0.0f);
	m_positionYMax.set	("ymax", 		20.0f, 0.0f, 50.0f);

	m_positionZMin.set	("zmin", 		0.0f, 10.0f, 20.0f);
	m_positionZMax.set	("zmax", 		20.0f, 20.0f, 100.0f);

	m_sizeMin.set		("sizemin", 	5.0f, 1.0f, 10.0f);
	m_sizeMax.set		("sizemax", 	12.0f, 10.0f, 30.0f);

	m_parameters.add(m_positionXMin);
	m_parameters.add(m_positionXMax);

	m_parameters.add(m_positionYMin);
	m_parameters.add(m_positionYMax);

	m_parameters.add(m_positionZMin);
	m_parameters.add(m_positionZMax);

	m_parameters.add(m_sizeMin);
	m_parameters.add(m_sizeMax);
	
	m_nbParticles = 10;

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

		for (int i=0;i<m_nbParticles;i++)
		{
			ofIcoSpherePrimitive* pPrimitive = new ofIcoSpherePrimitive( ofRandom(m_sizeMin,m_sizeMax), 0 );
			// pPrimitive->setPosition(ofRandom(-30,30), ofRandom(-30,30), ofRandom(m_positionZMin,m_positionZMax));
			m_particles.push_back( pPrimitive );
	 

			int nbVertices = pPrimitive->getMesh().getVertices().size();
			float dx = ofRandom(m_positionXMin,m_positionXMax);
			float dy = ofRandom(m_positionYMin,m_positionYMax);
			float dz = ofRandom(m_positionZMin,m_positionZMax);
			for (int k=0; k<nbVertices ; k++)
			{
				ofVec3f& v = pPrimitive->getMesh().getVertices()[k];
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
