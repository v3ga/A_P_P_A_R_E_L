//
//  meshVertexSelector.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 31/05/2014.
//
//

#include "meshVertexSelector.h"

meshVertexSelector::meshVertexSelector()
{
	m_thDistVertex = 10.0f;
}

meshFaceSelectorResult*	meshVertexSelector::select(const ofMesh& mesh, const ofCamera& cam, const ofVec2f& point)
{
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	result.vertexIndex = 0;
	result.vertex = 0;
	
	ofVec2f v2d;
	ofVec3f normal;
	ofVec3f dir;
	ofVec3f v;

	for(int i = 0; i < n; i++)
	{
		v = mesh.getVertex(i);
		dir = v - cam.getPosition();
		normal = mesh.getNormal(i);
	
		v2d = cam.worldToScreen(v);
		float distance = v2d.distance(point);
		if((i == 0 || distance < nearestDistance))
		{
			nearestDistance = distance;
			
			if (distance <= m_thDistVertex && dir.dot(normal)<0.0f)
			{
				// result.nearestVertex = v;
				result.vertex = (ofVec3f*)mesh.getVerticesPointer() + i;
				result.vertexScreen = v2d;
				result.vertexIndex = i;

			}
		}
	}
	return &result;
}

