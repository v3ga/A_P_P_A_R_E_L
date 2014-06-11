//
//  meshFaceSelector.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 31/05/2014.
//
//

#include "meshFaceSelector.h"


//--------------------------------------------------------------
meshFaceSelectorResult* meshFaceSelector::select(const vector<ofMeshFace>& meshFaces, const ofCamera& cam, const ofVec2f& point)
{
	ofVec3f 	pointWorld = cam.screenToWorld(ofVec3f(point.x, point.y, 0.0));
    ofRay 		ray(cam.getPosition(),pointWorld - cam.getPosition());
	ofMeshFace 	face;
	ofPlane		facePlane;
	ofVec3f		faceNormal, faceNormalCam;
	ofVec3f 	pointOnPlane;
	ofMeshFace*	pMeshFace = 0;
	
	result.face = 0;
	result.faceIndex = -1;
	result.vertex = 0;

	for (int i=0; i<meshFaces.size(); i++)
	{
		face = meshFaces[i];
		faceNormal = face.getFaceNormal();
		
		facePlane.setCenter(face.getVertex(0));
		facePlane.setNormal(faceNormal);
		
		// Intersection with plane
	   	if ( facePlane.intersect(ray,pointOnPlane) )
		{
			// Point in triangle ?
			if ( isPointInTriangle(face.getVertex(0),face.getVertex(1),face.getVertex(2), pointOnPlane) )
			{
				ofVec3f dir = pointOnPlane - cam.getPosition();

				if (dir.dot(faceNormal)<0.0f){
					pMeshFace =  (ofMeshFace*) &meshFaces[i];
					result.face = pMeshFace;
					result.faceIndex = i;
					
					onMeshFaceSelected(pMeshFace, pointOnPlane);
					break;
				}
			}
		}
	}
	
	return &result;
}

//--------------------------------------------------------------
// http://www.blackpawn.com/texts/pointinpoly/
bool meshFaceSelector::isPointInTriangle(const ofVec3f& A, const ofVec3f& B, const ofVec3f& C, const ofVec3f& P)
{
	ofVec3f v0 = C-A;
	ofVec3f v1 = B-A;
	ofVec3f v2 = P-A;

	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);
}