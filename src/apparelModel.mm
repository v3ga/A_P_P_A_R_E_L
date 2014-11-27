//
//  apparelModel.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 19/05/2014.
//
//

#include "apparelModel.h"
#include "globals.h"
#include "ofxXmlSettings.h"
#ifdef TARGET_OF_IOS
#include "ofxIOSExtras.h"
#endif


//--------------------------------------------------------------
apparelModel::apparelModel()
{
	id = "";
	assimpModel.setScaleNomalization(false);
	scale.set(1.0f);
}

//--------------------------------------------------------------
bool apparelModel::load(string modelName, bool optimize)
{
 	bool bLoaded = assimpModel.loadModel(getPathRelative(modelName), optimize);

   	OFAPPLOG->println("- loading "+getPathRelative(modelName));

	if (bLoaded){
		id = modelName;
		mesh = assimpModel.getMesh(0);
		mesh.disableTextures();
		createMeshFaces();
		
	}
	return bLoaded;
}

//--------------------------------------------------------------
bool apparelModel::loadProperties()
{
	ofxXmlSettings properties;
	if ( properties.load( getPathRelative( getPropertiesFilename() )) )
	{
		ofVec3f position;
	 

		ofVec3f scale;
	}
	return false;
}



//--------------------------------------------------------------
bool apparelModel::saveProperties()
{
	OFAPPLOG->begin("apparelModel::saveProperties()");

	ofxXmlSettings properties;
	
	properties.addTag("properties");
	properties.pushTag("properties");
		properties.addTag("position");
		properties.pushTag("position");
			properties.addValue("x", assimpModel.getPosition().x);
			properties.addValue("y", assimpModel.getPosition().y);
			properties.addValue("z", assimpModel.getPosition().z);
		properties.popTag();

		properties.addValue("scale", assimpModel.getScale().x);

	properties.popTag();


	// Create 3d directory
/*	string path3d = getPathDocument();
	OFAPPLOG->println("- pathUser="+pathUser);
	ofDirectory dirUser(pathUser);
	if (!dirUser.exists())
	{
		OFAPPLOG->println("- creating it");
		dirUser.create(true);
	}
*/



	string pathProperties = getPathDocument(getPropertiesFilename());
	bool saved = properties.save(pathProperties);

	OFAPPLOG->println("saved ok to "+pathProperties+"? "+ofToString(saved));
	OFAPPLOG->end();

	return saved;
}

//--------------------------------------------------------------
void apparelModel::drawFaces()
{
	ofPushMatrix();
	ofMultMatrix(modelMatrix);
	mesh.drawFaces();
	ofPopMatrix();
}

//--------------------------------------------------------------
void apparelModel::drawWireframe()
{
	ofPushMatrix();
	ofMultMatrix(modelMatrix);
	mesh.drawWireframe();
	ofPopMatrix();
}


//--------------------------------------------------------------
string apparelModel::getPathRelative(string filename)
{
	return "3d/"+filename;
}

//--------------------------------------------------------------
string apparelModel::getPropertiesFilename()
{
	// ofLog()<<id;
	vector<string> idWithoutExt = ofSplitString(id, ".");
	return idWithoutExt[0]+"_properties.xml";
}

//--------------------------------------------------------------
string apparelModel::getPathDocument(string filename)
{
	#ifdef TARGET_OF_IOS
		return ofxiOSGetDocumentsDirectory() + getPathRelative(filename);
	#else
		return ofToDataPath(getPathRelative(filename));
	#endif
}


//--------------------------------------------------------------
void apparelModel::createMeshFaces()
{
	meshFaces.clear();
	meshFacesIndices.clear();

	vector<ofIndexType>& 	indices 	= mesh.getIndices();
	vector<ofVec3f>&		vertices	= mesh.getVertices();

	for (int i=0; i<indices.size();i+=3)
	{
		ofMeshFace face;

		face.setVertex( 0, vertices[indices[i+0]] );
		face.setVertex( 1, vertices[indices[i+1]] );
		face.setVertex( 2, vertices[indices[i+2]] );

		meshFaces.push_back(face);
		meshFacesIndices.push_back( ofMeshFaceIndices( indices[i+0], indices[i+1], indices[i+2] ) );

		face.getFaceNormal();
	}
	
}

//--------------------------------------------------------------
void apparelModel::removeMeshFace(int index)
{
	if (index < meshFaces.size())
	{
		ofMeshFace 			faceToRemove = meshFaces[index];
		ofMeshFaceIndices	faceIndicesToRemove = meshFacesIndices[index];
	
		// Remove indices from mesh
		mesh.removeIndex( faceIndicesToRemove.m_index[0] );
		mesh.removeIndex( faceIndicesToRemove.m_index[1] );
		mesh.removeIndex( faceIndicesToRemove.m_index[2] );

		// Remove instances from list
	 	meshFacesIndices.erase(meshFacesIndices.begin()+index);
	 	meshFaces.erase(meshFaces.begin()+index);
	}
	else
	{
		OFAPPLOG->begin("apparelModel::removeMeshFace(\""+ofToString(index)+"\")");
		OFAPPLOG->println(ofToString(index)+" is not a valid index (max is "+ofToString(meshFaces.size()-1)+")");
		OFAPPLOG->end();
	}
	
}


//--------------------------------------------------------------
void apparelModel::setScale(float x, float y, float z)
{
	scale.set( ofVec3f(x,y,z) );
	updateModelMatrix();
}

//--------------------------------------------------------------
void apparelModel::setPosition(float x, float y, float z)
{
	pos.set( ofVec3f(x,y,z) );
	updateModelMatrix();
}

//--------------------------------------------------------------
void apparelModel::updateModelMatrix()
{
    modelMatrix.makeIdentityMatrix();
    modelMatrix.glTranslate(pos);
    //modelMatrix.glRotate(180, 0, 0, 1);
    //if(assimpModel.normalizeScale)
	{
        //modelMatrix.glScale(assimpModel.getNormalizedScale() , assimpModel.getNormalizedScale(), assimpModel.getNormalizedScale());
    }
/*    for(int i = 0; i < (int)rotAngle.size(); i++){ // @julapy - not sure why rotAngle isn't a ofVec4f.
        modelMatrix.glRotate(rotAngle[i], rotAxis[i].x, rotAxis[i].y, rotAxis[i].z);
    }
*/
    modelMatrix.glScale(scale.x, scale.y, scale.z);
}


//--------------------------------------------------------------
void apparelModel::copyMeshAndTransformation(const apparelModel& otherModel)
{
	OFAPPLOG->begin("apparelModel::copyMeshAndTransformation()");

	id 		= otherModel.id;
	mesh 	= otherModel.mesh;
	pos 	= otherModel.pos;
	scale 	= otherModel.scale;

	createMeshFaces();
	updateModelMatrix();

	OFAPPLOG->println(" - mesh (copy) has "+ofToString(mesh.getNumVertices())+" vertices & "+ofToString(mesh.getNumIndices())+" indices");
	OFAPPLOG->end();
}


