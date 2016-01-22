//
//  apparelMood_noisopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/11/2014.
//
//

#include "apparelMood_noisopathy.h"
#include "apparelMod_zoopathy.h"
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
}

//--------------------------------------------------------------
void apparelMood_noisopathy::update()
{
	float time = ofGetElapsedTimef();
    float timeScale = m_timeScale;
    float displacementScale = m_weight*m_amplitude;
	
	int numVerts = (int)m_meshInput.getNumVertices();
	ofVec3f vert;
	ofVec3f timeOffsets;
	for (int i=0; i<numVerts; ++i)
	{
    	vert = m_meshInput.getVertex(i);
    	timeOffsets = m_offsets[i];

		vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
    	vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
    	vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;

    	m_model.mesh.setVertex(i, vert);
	}
}

//--------------------------------------------------------------
void apparelMood_noisopathy::updateModZoopathy(apparelMod_zoopathy* pModZoo)
{
	if (pModZoo==0) return;

	float time = ofGetElapsedTimef();
    float timeScale = m_timeScale;
    float displacementScale = m_weight*m_amplitude;

//	pModZoo->setChanged();
	pModZoo->m_bDoUpdatePoils = true;
	
	vector<Poil*>::iterator it = pModZoo->m_poils.begin();
	ofVec3f vert;
	ofVec3f timeOffsets;
	for ( ; it != pModZoo->m_poils.end(); ++it)
	{
		int n = (*it)->m_resolution;
		for (int i=1;i<n;i++)
		{
	    	timeOffsets = m_offsets[i%n];

			(*it)->m_line[i].x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
    		(*it)->m_line[i].y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
    		(*it)->m_line[i].z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
		}
	}
}

