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
Poil::Poil(int res, float rnd)
{
	m_resolution = res;
	m_indexFace = 0;
	for (int i=0;i<m_resolution;i++)
	{
		m_line.addVertex(ofVec3f());
		m_random.push_back(ofVec3f(ofRandom(rnd),ofRandom(rnd),ofRandom(rnd)));
	}
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
	m_nbFaceSelfo = 0;
	m_bDoUpdatePoils = false;
	m_bDoCreatePoils = false;

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod_zoopathy::createParameters()
{
	apparelMod::createParameters();

	m_lengthMax.set("LengthMax", 20.0f, 5.0f, 60.0f);
	m_density.set("Density", 30.0f, 1.0f, 10.0f);
	m_random.set("Random", 0.5f,0.0f,1.0f);
	m_resolution.set("Resolution", 2, 2, 30);
	parameterGroupLowHigh::create(m_zPoils, "zPoils",-60.0f,-20.0f,20.0f,60.0f);

	m_parameters.add(m_lengthMax);
//	m_parameters.add(m_density);
	m_parameters.add(m_resolution);
	m_parameters.add(m_zPoils);
	m_parameters.add(m_random);
}

//--------------------------------------------------------------
void apparelMod_zoopathy::apply()
{
}

//--------------------------------------------------------------
void apparelMod_zoopathy::update()
{
	apparelMod_selfopathy* pModSelfopathy = GLOBALS->mp_modSelfopathy;//(apparelMod_selfopathy*) GLOBALS->getModManager()->getMod("");

	if (pModSelfopathy)
	{
		vector<ofMeshFaceApparel*>& faces = pModSelfopathy->m_model.getMeshFacesRef();
		int nbFaces = faces.size();

		if (m_bDoCreatePoils)
		{
			m_nbFaceSelfo = nbFaces;
			m_bDoCreatePoils = false;

		   deletePoils();
	   
		   vector<ofVec3f>& vertices = m_model.getVerticesRef();
		   vector<ofVec3f>& normals = m_model.getNormalsRef();

		   
		   ofVec3f centroid, nFace;

		   for (int i=0;i<nbFaces;i++)
		   {
			   ofMeshFaceApparel* pFace = faces[ i/*m_indicesFaces[i]*/ ];
			
			   //I = nFace.crossed(X);
			   nFace = pFace->getFaceNormal();

			   // Centroid
			   ofVec3f* pVertex0 = pFace->getVertexPointer(0);
			   ofVec3f* pVertex1 = pFace->getVertexPointer(1);
			   ofVec3f* pVertex2 = pFace->getVertexPointer(2);
	   
			   centroid.x = (pVertex0->x + pVertex1->x + pVertex2->x)/3.0;
			   centroid.y = (pVertex0->y + pVertex1->y + pVertex2->y)/3.0;
			   centroid.z = (pVertex0->z + pVertex1->z + pVertex2->z)/3.0;

			   if (centroid.z >= m_zPoils.getFloat("low") && centroid.z <= m_zPoils.getFloat("high"))
			   {
				   Poil* pPoil = new Poil(m_resolution, m_random);
				   pPoil->m_origin = centroid;
				   pPoil->m_normal = nFace;
				   m_poils.push_back(pPoil);
			   
			   }
		   	}
		 } // if (nbFaces != m_nbFaceSelfo)
	}

		if (m_bDoUpdatePoils)
		{
		 	vector<Poil*>::iterator it = m_poils.begin();
		 
		   float poilLength = m_weight * m_lengthMax;
			
		   ofVec3f v,dv;

		   Poil* pPoil = 0;
		   for ( ; it != m_poils.end(); ++it)
		   {
			 pPoil = *it;
			 pPoil->m_line[0] = pPoil->m_origin;
			 for (int j=1;j<m_resolution;j++)
			 {
			    dv = pPoil->m_normal * poilLength / (float)(m_resolution-1);
				pPoil->m_line[j] = pPoil->m_line[j-1] + dv*pPoil->m_random[j];
			  }
		   }
		}
}

//--------------------------------------------------------------
void apparelMod_zoopathy::drawExtra()
{
	if (m_weight>0.0f)
	{
		vector<Poil*>::iterator it = m_poils.begin();
		for ( ; it!=m_poils.end();++it)
			(*it)->draw();
	}
	
	if (m_bDrawDebug)
	{
		ofPushStyle();
		ofSetColor(255.0f,255.0f,0.0f);
		ofLine(-60.0f,0.0f,m_zPoils.getFloat("low"),60.0f,0.0f,m_zPoils.getFloat("low"));

		ofSetColor(255.0f,0.0f,255.0f);
		ofLine(-60.0f,0.0f,m_zPoils.getFloat("high"),60.0f,0.0f,m_zPoils.getFloat("high"));
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void apparelMod_zoopathy::deletePoils()
{
	vector<Poil*>::iterator it = m_poils.begin();
	for ( ; it!=m_poils.end();++it)
		delete *it;
	m_poils.clear();
}

//--------------------------------------------------------------
void apparelMod_zoopathy::onParameterChanged(ofAbstractParameter& parameter)
{
//  setChanged();

  m_bDoUpdatePoils = true;
  if (parameter.getName() == "low" || parameter.getName() == "high" || parameter.getName() == "Resolution" || parameter.getName() == "Random"){
  	m_bDoCreatePoils = true;
  }
}

//--------------------------------------------------------------
void apparelMod_zoopathy::onWeightChanged()
{
  m_bDoUpdatePoils = true;
}



