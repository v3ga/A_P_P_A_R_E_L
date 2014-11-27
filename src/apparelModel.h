//
//  apparelModel.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 19/05/2014.
//
//

#pragma once
#include "ofxAssimpModelLoader.h"

class ofMeshFaceIndices
{
	public:
		ofMeshFaceIndices	(int index0,int index1,int index2){m_index[0]=index0;m_index[1]=index1;m_index[2]=index2;}
		ofMeshFaceIndices& operator=(const ofMeshFaceIndices& other)
		{
			m_index[0] = other.m_index[0];
			m_index[1] = other.m_index[1];
			m_index[2] = other.m_index[2];
			
			return *this;
		}

		int					m_index[3];
};

class apparelModel
{
	public:
		apparelModel	();
	

        bool 				load			(string modelName, bool optimize=false);
		bool				loadProperties	();
		bool				saveProperties	();

		void				drawFaces		();
		void				drawWireframe	();
 

        void 				setScale		(float x, float y, float z);
        void 				setPosition		(float x, float y, float z);
		void				copyMeshAndTransformation(const apparelModel& otherModel);

		string				getId			(){return id;}
		ofMesh&				getMeshRef		(){return mesh;}
		vector<ofMeshFace>&	getMeshFacesRef	(){return meshFaces;}
		vector<ofVec3f>& 	getVerticesRef	(){return mesh.getVertices();}
		vector<ofVec3f>& 	getNormalsRef	(){return mesh.getNormals();}

		string						id;
		ofMesh						mesh;
		vector<ofMeshFace>			meshFaces;
		vector<ofMeshFaceIndices>	meshFacesIndices;
 

		void				removeMeshFace			(int index);
		void				createMeshFaces			();

	private:
		ofxAssimpModelLoader assimpModel;

		void				updateModelMatrix		(); // directly copied from ofxAssimpModelLoader
 
		string				getPathRelative			(string filename);
		string				getPathDocument			(string filename);
		string				getPropertiesFilename	();


        ofPoint 			scale;
        ofPoint 			pos;
        ofMatrix4x4 		modelMatrix;
};
