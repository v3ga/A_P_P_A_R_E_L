//
//  apparelMood_sad.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMood_sad.h"
#include "apparelMod_selfopathy.h"
#include "globals.h"



//--------------------------------------------------------------
Drop::Drop(apparelMood_sad* pParent, ofVec3f p)
{
	mp_parent = pParent;
	m_posEmit = m_posA = m_posB = p;
	m_bFollow = false;
}

//--------------------------------------------------------------
void Drop::update(float dt)
{
	m_posA.z -= m_speed*dt;

	float d = abs(m_posEmit.z - m_posA.z);
	if (d>m_length)
	{
	   m_posB.z = m_posA.z + m_length;
	}
	else
	{
		m_posB.z = m_posEmit.z;
	}
	
	if (m_posA.z <= m_posEmit.z + mp_parent->m_zRemoveDrop)
	{
		m_posA = m_posB = m_posEmit;
	}
}

//--------------------------------------------------------------
void Drop::draw()
{
	ofLine(m_posA,m_posB);
}


//--------------------------------------------------------------
apparelMood_sad::apparelMood_sad() : apparelMod("Sad")
{
	m_isMood = true;
}

//--------------------------------------------------------------
void apparelMood_sad::createParameters()
{
	m_zEmitDrop.set		("zEmitDrop", 		-10.0f,-50.0f,50.0f);
	m_zRemoveDrop.set	("zRemoveDrop", 	-50.0f,-100.0f,-50.0f);
	m_zSpeedDrop.set	("zSpeedDrop", 		30.0f,10.0f,60.0f);
	m_lengthDrop.set	("lengthDrop", 		10.0f,1.0f,30.0f);
	

	m_parameters.add(m_zEmitDrop);
	m_parameters.add(m_zRemoveDrop);
	m_parameters.add(m_zSpeedDrop);
	m_parameters.add(m_lengthDrop);
}

//--------------------------------------------------------------
void apparelMood_sad::apply()
{

	deleteDrops();

	apparelMod_selfopathy* pModSelfopathy = GLOBALS->mp_modSelfopathy;
	if (pModSelfopathy)
	{
		apparelModel& model = pModSelfopathy->m_model;
		
		ofVec3f* pVertices = model.getMeshRef().getVerticesPointer();
		int nbVertices = model.getMeshRef().getNumVertices();
		
		ofVec3f* pVertex=0;
		for (int i=0;i<nbVertices;i++)
		{
			pVertex = pVertices+i;
			if (pVertex->z <= m_zEmitDrop)
			{
				Drop* pDrop = new Drop(this, *pVertex);
				pDrop->m_length = ofRandom(0.5f*m_lengthDrop,m_lengthDrop);
				pDrop->m_speed 	= ofRandom(0.5f*m_zSpeedDrop,m_zSpeedDrop);
				
				m_drops.push_back(pDrop);
			}
		}
	}
}

//--------------------------------------------------------------
void apparelMood_sad::update()
{
	float dt = ofGetLastFrameTime();
	vector<Drop*>::iterator it = m_drops.begin();
	for ( ; it!=m_drops.end(); ++it)
		(*it)->update(dt);
}

//--------------------------------------------------------------
void apparelMood_sad::drawExtra()
{
	if (m_bDrawDebug)
	{
		ofPushStyle();
		ofDrawAxis(60.0f);
		ofSetColor(255.0f,255.0f,0.0f);
		ofLine(-60.0f,0.0f,m_zEmitDrop,60.0f,0.0f,m_zEmitDrop);

		ofSetColor(255.0f,0.0f,255.0f);
		ofLine(-60.0f,0.0f,m_zRemoveDrop,60.0f,0.0f,m_zRemoveDrop);
		ofPopStyle();
	}


	vector<Drop*>::iterator it = m_drops.begin();
	for ( ; it!=m_drops.end(); ++it)
		(*it)->draw();
}


//--------------------------------------------------------------
void apparelMood_sad::copyModelFrom(const apparelModel& model)
{
	apparelMod::copyModelFrom(model);
}

//--------------------------------------------------------------
void apparelMood_sad::deleteDrops()
{
	vector<Drop*>::iterator it = m_drops.begin();
	for ( ; it!=m_drops.end(); ++it)
		delete *it;
	m_drops.clear();
}

//--------------------------------------------------------------
void apparelMood_sad::onParameterChanged(ofAbstractParameter& parameter)
{
	if (parameter.getName() == "zEmitDrop" || parameter.getName() == "zSpeedDrop" || parameter.getName() == "lengthDrop")
	{
		setChanged();
	}
}

