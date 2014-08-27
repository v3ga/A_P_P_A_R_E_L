//
//  apparelMod_girliness.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/07/2014.
//
//

#include "apparelMod_girliness.h"
#include "apparelModel.h"

apparelMod_girliness :: apparelMod_girliness() : apparelMod("Girliness")
{
	
}


void apparelMod_girliness::draw()
{
	if (!m_isActive) 	return;
	if (!mp_model) 		return;


	vector<ofMeshFace>&	modelFaces = mp_model->getMeshFacesRef();
	int nbModelFaces = modelFaces.size();

	ofVec3f middleFace;
	ofVec3f normalFace;
	float breastHeight = m_weight * 30.0f;
	
	int nbFaces = m_indicesFaces.size();

	for (int i=0; i<nbFaces; i++)
	{
		int indexFace = m_indicesFaces[i];
		// Safety check
		if (indexFace < nbModelFaces)
		{
			ofMeshFace face = modelFaces[indexFace];
			middleFace 	= (face.getVertex(0) + face.getVertex(1) + face.getVertex(2))/3;
			normalFace = face.getFaceNormal();

			ofSetColor(255);
			ofNoFill();
			ofVec3f C = middleFace + normalFace*breastHeight;
			ofVec3f A,B;
			for (int j=0; j<3 ; j++)
			{
			  	A = face.getVertex(j);
			  	B = face.getVertex((j+1)%3);
				ofTriangle(A,B,C);
			}
			ofLine(middleFace,C);

		}
	}


}
