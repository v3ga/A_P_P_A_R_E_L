//
//  apparelModel.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 19/05/2014.
//
//

#pragma once
#include "ofxAssimpModelLoader.h"

class apparelModel : public ofxAssimpModelLoader
{
	public:
		apparelModel	();
	

        bool 				load			(string modelName, bool optimize=false);
        void 				setRotation		(float angle, float rot_x, float rot_y, float r_z);

		string				getId			(){return id;}
		ofMesh&				getMeshRef		(){return mesh;}
		vector<ofMeshFace>&	getMeshFacesRef	(){return meshFaces;}
		vector<ofVec3f>& 	getVerticesRef	(){return mesh.getVertices();}
		vector<ofVec3f>& 	getNormalsRef	(){return mesh.getNormals();}

		string				id;
		ofMesh				mesh;
		vector<ofMeshFace>	meshFaces;

	private:
		void				createMeshFaces	();
};
