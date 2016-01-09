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
void apparelMood_porcupinopathy::createParameters()
{
	m_amplitude.set("Amplitude", 5.0f, 0.0f, 20.0f);

	m_parameters.add(m_amplitude);
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::apply()
{
	apparelMod_selfopathy* pModSelfopathy = GLOBALS->mp_modSelfopathy;

	m_model.mesh = m_meshInput;

	if (pModSelfopathy)
	{
		m_meshSpikes.clear();
		deleteVerticesPorcu();
	
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
//			M = M + 15*n;
			
			int nbIndices = m_meshSpikes.getNumVertices();

			m_meshSpikes.addVertex( A );
			m_meshSpikes.addVertex( B );
			m_meshSpikes.addVertex( C );
			
			m_meshSpikes.addVertex( M );
			
			m_verticesPorcu.push_back( new PorcuVertex(M,n,m_meshSpikes.getNumVertices()-1 ) );
			
			
/*			m_meshSpikes.addIndex( nbIndices );
			m_meshSpikes.addIndex( nbIndices+1 );
			m_meshSpikes.addIndex( nbIndices+2 );
*/

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

		int nbMeshVertices = m_model.mesh.getNumVertices();

		m_model.mesh.append( m_meshSpikes );
		m_model.createMeshFaces();

		for (int i=0; i<m_verticesPorcu.size(); i++ )
		{
			 m_verticesPorcu[i]->m_index+=nbMeshVertices;
		}
	}


	if (isChanged())
	{
//		m_model.mesh = m_meshInput;
	}
}

//--------------------------------------------------------------
void apparelMood_porcupinopathy::onParameterChanged(ofAbstractParameter& parameter)
{
}


//--------------------------------------------------------------
void apparelMood_porcupinopathy::update()
{
	// Get amplitude of sound
	float soundVolume = GLOBALS->getSoundInputVolume(); // ofMap(GLOBALS->getSoundInputVolume(),0.0f,0.2f, 0.1f,1.0f);
	
	
	ofVec3f* pVertex = 0;
	for (int i=0; i<m_verticesPorcu.size(); i++ )
	{
		 pVertex = m_model.mesh.getVerticesPointer() + m_verticesPorcu[i]->m_index;
	 	 *pVertex = m_verticesPorcu[i]->m_pos + soundVolume * m_amplitude * m_verticesPorcu[i]->m_normal;
	
//		m_vertexSpikeIndices[i]+=nbMeshVertices;
	}
}

//--------------------------------------------------------------
void apparelMood_porcupinopathy::deleteVerticesPorcu()
{
	vector<PorcuVertex*>::iterator it = m_verticesPorcu.begin();
	for ( ; it!=m_verticesPorcu.end(); ++it)
	{
		delete *it;
	}
	m_verticesPorcu.clear();
}

