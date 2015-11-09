//
//  apparelMode_porcupinopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#include "apparelMood_porcupinopathy.h"
#include "ofAppLog.h"
#include "globals.h"
#include "parameterGroupLowHigh.h"
#include "apparelMod_selfopathy.h"
#include "apparelModManager.h"

//--------------------------------------------------------------
apparelMood_porcupinopathy::apparelMood_porcupinopathy() : apparelMod("Porcupinopathy")
{
	m_isMood = true;
}

//--------------------------------------------------------------
apparelMood_porcupinopathy::~apparelMood_porcupinopathy()
{
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::apply()
{
	apparelMod_selfopathy* pModSelfopathy = GLOBALS->mp_modSelfopathy;//(apparelMod_selfopathy*) GLOBALS->getModManager()->getMod("");

	m_model.mesh = m_meshInput;

	if (pModSelfopathy)
	{
		m_meshSpikes.clear();
	
		apparelModel& model = pModSelfopathy->m_model;
		
		vector<ofIndexType>& indices = model.getMeshRef().getIndices();
		ofVec3f A,B,C,M,n;
		for (int i=0;i<indices.size(); i=i+3)
		{
			ofIndexType indexA = indices[i];
			ofIndexType indexB = indices[i+1];
			ofIndexType indexC = indices[i+2];

			A = model.getMeshRef().getVertex( indexA );
			B = model.getMeshRef().getVertex( indexB );
			C = model.getMeshRef().getVertex( indexC );

			M = (A+B+C)/3.0f;

			n = (B-A).cross(C-A).normalize();
			M = M + 5*n;
			
			int nbIndices = m_meshSpikes.getNumVertices();

			m_meshSpikes.addVertex( A );
			m_meshSpikes.addVertex( B );
			m_meshSpikes.addVertex( C );
			
			m_meshSpikes.addVertex( M );
			

			
			m_meshSpikes.addIndex( nbIndices );
			m_meshSpikes.addIndex( nbIndices+1 );
			m_meshSpikes.addIndex( nbIndices+2 );

			m_meshSpikes.addIndex( nbIndices );
			m_meshSpikes.addIndex( nbIndices+1 );
			m_meshSpikes.addIndex( nbIndices+3 );

			m_meshSpikes.addIndex( nbIndices+1 );
			m_meshSpikes.addIndex( nbIndices+2 );
			m_meshSpikes.addIndex( nbIndices+3 );

			m_meshSpikes.addIndex( nbIndices+2 );
			m_meshSpikes.addIndex( nbIndices+0 );
			m_meshSpikes.addIndex( nbIndices+3 );

		}

		m_model.mesh.append( m_meshSpikes );
	}


	if (isChanged())
	{
//		m_model.mesh = m_meshInput;
	}
}

//--------------------------------------------------------------
void apparelMood_porcupinopathy::onParameterChanged(ofAbstractParameter& parameter)
{
	// Only recompute Mesh if subdivision is changed
/*	if (parameter.getName() == "Subdivision")
		setChanged(true);
*/
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::update()
{
	// Get amplitude of sound
	//m_weight = GLOBALS->getSoundInputVolume(); // ofMap(GLOBALS->getSoundInputVolume(),0.0f,0.2f, 0.1f,1.0f);
	
	
}

