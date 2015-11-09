//
//  ofMeshFaceApparel.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 04/12/2014.
//
//

#include "ofMeshFaceApparel.h"


//--------------------------------------------------------------
ofMeshFaceApparel::ofMeshFaceApparel(ofMesh* pMesh, int offsetIndices)
{
	mp_parent		= 0;
	mp_mesh 		= pMesh;
	m_offsetIndices = offsetIndices;
	
	setVertex(0, *getVertexPointer(0));
	setVertex(1, *getVertexPointer(1));
	setVertex(2, *getVertexPointer(2));
	
	getFaceNormal();
}


//--------------------------------------------------------------
ofIndexType ofMeshFaceApparel::getVertexIndex(int index)
{
	int offset = m_offsetIndices + index;
	if (offset<mp_mesh->getIndices().size())
	{
		return mp_mesh->getIndices()[offset];
	}
	else
	{
		ofLog(OF_LOG_WARNING) << "ofMeshFaceApparel::getVertexIndex("<<ofToString(offset)<<") offset out of range";
	}

	return 0;
}

//--------------------------------------------------------------
ofVec3f* ofMeshFaceApparel::getVertexPointer(int index)
{
	return mp_mesh->getVerticesPointer()+getVertexIndex(index);
}
