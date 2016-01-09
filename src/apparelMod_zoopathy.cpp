//
//  apparelMod_zoopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMod_zoopathy.h"
#include "apparelMod_selfopathy.h"
#include "ofAppLog.h"
#include "globals.h"


//--------------------------------------------------------------
Poil::Poil()
{
	m_indexFace = 0;
}

//--------------------------------------------------------------
void Poil::draw()
{
	m_line.draw();
}


//--------------------------------------------------------------
apparelMod_zoopathy::apparelMod_zoopathy() : apparelMod("Zoopathy")
{
	OFAPPLOG->begin("apparelMode_zoopathy::apparelMode_zoopathy()");

	m_density = 1;
	m_nbPoints = (int)ofRandom(5,10);

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod_zoopathy::createParameters()
{
	apparelMod::createParameters();

	m_lengthMax.set("LengthMax", 20.0f, 5.0f, 30.0f);
	m_density.set("Density", 30.0f, 1.0f, 10.0f);
	m_resolution.set("Resolution", 2.0f, 2.0f, 30.0f);

	m_parameters.add(m_lengthMax);
	m_parameters.add(m_density);
	m_parameters.add(m_resolution);
}

//--------------------------------------------------------------
void apparelMod_zoopathy::apply()
{
	apparelMod_selfopathy* pModSelfopathy = GLOBALS->mp_modSelfopathy;//(apparelMod_selfopathy*) GLOBALS->getModManager()->getMod("");

	if (pModSelfopathy)
	{
		deletePoils();
	
		vector<ofVec3f>& vertices = m_model.getVerticesRef();
		vector<ofVec3f>& normals = m_model.getNormalsRef();

		vector<ofMeshFaceApparel*>& faces = pModSelfopathy->m_model.getMeshFacesRef();
		int nbFaces = faces.size();//m_indicesFaces.size();
		
		ofVec3f centroid, nFace, v;
		ofVec3f X(-1.0f,0.0f,0.0f);
		ofVec3f I;

		float poilLength = m_weight * m_lengthMax;
		int poilNbPoints = (int) (m_weight * m_resolution);
		if (poilNbPoints<2) poilNbPoints=2;
		ofVec3f dv = nFace * poilLength / (float)poilNbPoints;

		for (int i=0;i<nbFaces;i++)
		{
			ofMeshFaceApparel* pFace = faces[ i/*m_indicesFaces[i]*/ ];

			nFace = pFace->getFaceNormal();
			I = nFace.crossed(X);

			// Centroid
			ofVec3f* pVertex0 = pFace->getVertexPointer(0);
			ofVec3f* pVertex1 = pFace->getVertexPointer(1);
			ofVec3f* pVertex2 = pFace->getVertexPointer(2);
	
			centroid.x = (pVertex0->x + pVertex1->x + pVertex2->x)/3.0;
			centroid.y = (pVertex0->y + pVertex1->y + pVertex2->y)/3.0;
			centroid.z = (pVertex0->z + pVertex1->z + pVertex2->z)/3.0;

			v = centroid;
			
			Poil* pPoil = new Poil();
		
			for (int j=0;j<poilNbPoints;j++)
			{
				pPoil->m_line.addVertex(v);
				I = dv*ofVec3f( ofRandom(1),ofRandom(1),ofRandom(1) );

				v+=dv;
			}

			m_poils.push_back(pPoil);
			
		}
	}
}

//--------------------------------------------------------------
void apparelMod_zoopathy::drawExtra()
{
	vector<Poil*>::iterator it = m_poils.begin();
	for ( ; it!=m_poils.end();++it)
		(*it)->draw();
}

//--------------------------------------------------------------
void apparelMod_zoopathy::deletePoils()
{
	vector<Poil*>::iterator it = m_poils.begin();
	for ( ; it!=m_poils.end();++it)
		delete *it;
	m_poils.clear();
}


