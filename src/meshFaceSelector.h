//
//  meshFaceSelector.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 31/05/2014.
//
//

#pragma once

#include "ofMain.h"
#include "ofxRay.h"
#include "ofMeshFaceApparel.h"

struct meshFaceSelectorResult
{
	ofMeshFaceApparel* 	face;
	int					faceIndex;

	ofVec3f*			vertex;
	ofVec2f				vertexScreen;
	int 				vertexIndex;
};

class meshFaceSelector
{
	public:
	
		virtual meshFaceSelectorResult*		select				(const vector<ofMeshFaceApparel*>& faces, const ofCamera& cam, const ofVec2f& point);
		virtual	void						onMeshFaceSelected	(ofMeshFaceApparel* face, const ofVec3f pointInFace){}

	protected:
		bool						isPointInTriangle	(const ofVec3f& A, const ofVec3f& B, const ofVec3f& C, const ofVec3f& P);
		meshFaceSelectorResult 		result;

};

