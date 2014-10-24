//
//  apparelModel.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 19/05/2014.
//
//

#include "apparelModel.h"
#include "globals.h"

//--------------------------------------------------------------
apparelModel::apparelModel()
{
	id = "";
	setScaleNomalization(false);
}

//--------------------------------------------------------------
bool apparelModel::load(string modelName, bool optimize)
{
 	bool bLoaded = loadModel(modelName, optimize);
	if (bLoaded){
		id = modelName;
		mesh = getMesh(0);
		createMeshFaces();
	}
	return bLoaded;
}



//--------------------------------------------------------------
void apparelModel::createMeshFaces()
{
	vector<ofIndexType>& 	indices 	= mesh.getIndices();
	vector<ofVec3f>&		vertices	= mesh.getVertices();

	for (int i=0; i<indices.size();i+=3)
	{
		ofMeshFace face;

		face.setVertex( 0, vertices[indices[i+0]] );
		face.setVertex( 1, vertices[indices[i+1]] );
		face.setVertex( 2, vertices[indices[i+2]] );

		meshFaces.push_back(face);

		face.getFaceNormal();
	}
	
}

