//
//  apparelMod_zoopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMod_zoopathy.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
apparelMod_zoopathy::apparelMod_zoopathy() : apparelMod("Zoopathy")
{
	OFAPPLOG->begin("apparelMode_zoopathy::apparelMode_zoopathy()");
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod_zoopathy::drawExtra()
{
	vector<ofMeshFaceApparel*>& faces = m_model.getMeshFacesRef();
	int nbFaces = faces.size();
	ofVec3f centroid;
	for (int i=0;i<nbFaces;i++)
	{
		ofMeshFaceApparel* pFace = faces[i];

		// Centroid
		ofVec3f* pVertex0 = pFace->getVertexPointer(0);
		ofVec3f* pVertex1 = pFace->getVertexPointer(1);
		ofVec3f* pVertex2 = pFace->getVertexPointer(2);
		
		centroid.x = (pVertex0->x + pVertex1->x + pVertex2->x)/3.0;
		centroid.y = (pVertex0->y + pVertex1->y + pVertex2->y)/3.0;
		centroid.z = (pVertex0->z + pVertex1->z + pVertex2->z)/3.0;
	 
		ofLine(centroid, centroid+50*pFace->getFaceNormal());

//		pFace->get
//		ofLine(  );
	}
}

