//
//  apparelMood_noisopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#include "apparelMood_noisopathy.h"
#include "globals.h"

//--------------------------------------------------------------
apparelMood_noisopathy::apparelMood_noisopathy() : apparelMod("Noisopathy")
{
	m_isMood = true;
}

//--------------------------------------------------------------
void apparelMood_noisopathy::createParameters()
{
	m_amplitude.set("Amplitude", 30.0f, 0.0f, 50.0f);
	m_timeScale.set("TimeScale", 10.0f, 5.0f, 50.0f);

	m_parameters.add(m_amplitude);
	m_parameters.add(m_timeScale);
}


//--------------------------------------------------------------
void apparelMood_noisopathy::copyModelFrom(const apparelModel& model)
{
	apparelMod::copyModelFrom(model);

	m_offsets.clear();
	for (int i=0;i<m_meshInput.getNumVertices();i++)
	{
		m_offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
	}
}

//--------------------------------------------------------------
void apparelMood_noisopathy::apply()
{
	// OFAPPLOG->begin("apparelMood_noisopathy::apply()");
	if (isChanged())
	{

// TODO : À revoir
// 
//		m_model.removeMeshFaces( m_indicesFaces );
/*		OFAPPLOG->println(" - changed ! ");

		int nbFacesSelected = m_indicesFaces.size();
		for (int i=0; i<nbFacesSelected; i++)
		{
			vector<ofMeshFaceIndices>& faceIndices = m_model.meshFacesIndices;
		
			m_model.removeMeshFace(m_indicesFaces[i]);
		}

		m_indicesFaces.clear();
		m_model.createMeshFaces();
*/
	}
//	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMood_noisopathy::update()
{
	float time = ofGetElapsedTimef();
    float timeScale = m_timeScale;
    float displacementScale = m_weight*m_amplitude;
	
	int numVerts = m_meshInput.getNumVertices();
	for (int i=0; i<numVerts; ++i)
	{
    	ofVec3f vert = m_meshInput.getVertex(i);
    	ofVec3f timeOffsets = m_offsets[i];

		//vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
    	//vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
    	vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;

    	m_model.mesh.setVertex(i, vert);
	}

	
	setChanged();
}

