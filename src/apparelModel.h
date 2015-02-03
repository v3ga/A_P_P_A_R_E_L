//
//  apparelModel.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 19/05/2014.
//
//

#pragma once
#include "ofxAssimpModelLoader.h"
#include "ofMeshFaceApparel.h"

class apparelModel
{
	public:
		apparelModel				();
		~apparelModel				();
	

        bool 						load			(string modelName, bool optimize=false);
		bool						loadProperties	();
		bool						saveProperties	();


		virtual void				drawFaces		();
		virtual void				drawWireframe	();
 

        void 						setScale		(float x, float y, float z);
        void 						setPosition		(float x, float y, float z);

		void						copyMeshAndTransformation	(const apparelModel& otherModel);
		void						copyVerticesPositions		(const apparelModel& otherModel);

		ofMatrix4x4&				getModelMatrix	(){return modelMatrix;}

		string						getId			(){return id;}
		ofMesh&						getMeshRef		(){return mesh;}
		vector<ofMeshFaceApparel*>&	getMeshFacesRef	(){return meshFaces;}
		vector<ofVec3f>& 			getVerticesRef	(){return mesh.getVertices();}
		vector<ofVec3f>& 			getNormalsRef	(){return mesh.getNormals();}
		vector<ofIndexType>&		getIndicesRef	(){return mesh.getIndices();}

		string						toString		();


		string						id;
		ofMesh						mesh;
		vector<ofMeshFaceApparel*>	meshFaces;
 

		void						createMeshFaces			();
		void						clearMeshFaces			();

		void						removeMeshFace			(int index);
		void						removeMeshFaces			(const vector<int>& listFaces);

	private:
		ofxAssimpModelLoader 		assimpModel;

		void						updateModelMatrix		(); // directly copied from ofxAssimpModelLoader
 
		string						getPathRelative			(string filename);
		string						getPathDocument			(string filename);
		string						getPropertiesFilename	();


        ofPoint 					scale;
        ofPoint 					pos;
        ofMatrix4x4 				modelMatrix;
};
