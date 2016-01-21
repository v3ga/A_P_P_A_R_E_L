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

#define APPARELMODEL_TOSTRING_LISTS 	1

//--------------------------------------------------------------
apparelModel::apparelModel()
{
	id = "";
	#ifdef OF_TARGET_OSX
	assimpModel.setScaleNomalization(false);
	#endif

	#ifdef OF_TARGET_IOS
	assimpModel.setScaleNormalization(false);
	#endif
	scale.set(1.0f);
}


//--------------------------------------------------------------
apparelModel::~apparelModel()
{
	clearMeshFaces();
}

//--------------------------------------------------------------
bool apparelModel::load(string modelName, bool optimize)
{
	bool bLoaded = false;
	#ifdef TARGET_OSX
 	bLoaded = assimpModel.loadModel(getPathRelative(modelName), optimize);

   	OFAPPLOG->println("- loading "+getPathRelative(modelName));

	if (bLoaded)
	{
		id = modelName;
		mesh = assimpModel.getMesh(0);


		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		mesh.enableIndices();
		mesh.disableColors();

		mesh.mergeDuplicateVertices();
		
		ofxXmlSettings modelXml;

		int nbVertices = mesh.getNumVertices();
		modelXml.addTag("vertices");
		modelXml.pushTag("vertices");
		for (int i=0;i<nbVertices;i++)
		{
			 modelXml.addTag("v");
			 modelXml.pushTag("v",i);
			 modelXml.addValue("x", mesh.getVertex(i).x);
			 modelXml.addValue("y", mesh.getVertex(i).y);
			 modelXml.addValue("z", mesh.getVertex(i).z);
			 modelXml.popTag();
		}
		modelXml.popTag();

		int nbNormals = mesh.getNumNormals();
		modelXml.addTag("normals");
		modelXml.pushTag("normals");
		for (int i=0;i<nbNormals;i++)
		{
			 modelXml.addTag("n");
			 modelXml.pushTag("n",i);
			 modelXml.addValue("x", mesh.getNormal(i).x);
			 modelXml.addValue("y", mesh.getNormal(i).y);
			 modelXml.addValue("z", mesh.getNormal(i).z);
			 modelXml.popTag();
		}
		modelXml.popTag();
		

		modelXml.addTag("indices");
		modelXml.pushTag("indices");
		int nbIndices = mesh.getNumIndices();
		for (int i=0;i<nbIndices;i++)
		{
			 modelXml.addValue("i", (int)mesh.getIndex(i));
		}
		modelXml.popTag();
		
		modelXml.saveFile("3d/AR_model_23p.xml");


		
//		mesh.disableTextures();
//		mesh.enableTextures();

		createMeshFaces();

		OFAPPLOG->println( toString() );
	}
	#endif

//	#ifdef TARGET_IOS
		ofxXmlSettings modelXml;

		bLoaded = modelXml.load("3d/AR_model_23p.xml");
		if (bLoaded)
		{
			mesh.clear();
			mesh.setMode(OF_PRIMITIVE_TRIANGLES);
			mesh.enableIndices();
			mesh.disableColors();

			modelXml.pushTag("vertices");
			int nbVertices = modelXml.getNumTags("v");
			for (int i=0; i<nbVertices; i++)
			{
				modelXml.pushTag("v",i);
				mesh.addVertex( ofVec3f( modelXml.getValue("x", 0.0f), modelXml.getValue("y", 0.0f), modelXml.getValue("z", 0.0f)  ) );
				modelXml.popTag();
			}
			modelXml.popTag();

			modelXml.pushTag("normals");
			int nbNormals = modelXml.getNumTags("n");
			for (int i=0; i<nbNormals; i++)
			{
				modelXml.pushTag("n",i);
				mesh.addNormal( ofVec3f( modelXml.getValue("x", 0.0f), modelXml.getValue("y", 0.0f), modelXml.getValue("z", 0.0f)  ) );
				modelXml.popTag();
			}
			modelXml.popTag();
		
		
			modelXml.pushTag("indices");
			int nbIndices = modelXml.getNumTags("i");
			for (int i=0; i<nbIndices; i++)
			{
				mesh.addIndex( modelXml.getValue("i",0,i) );
			}
			modelXml.popTag();

			createMeshFaces();

		OFAPPLOG->println( toString() );
		}



//	#endif


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

	// TEMP
/*	mesh.enableColors();

    if(!mesh.hasColors()) {
        mesh.getColors().resize( mesh.getNumVertices() );
    }

	vector<ofMeshFaceApparel*>::iterator it = meshFaces.begin();
	
	for ( ; it != meshFaces.end(); ++it )
	{
		ofMeshFaceApparel* pFace = *it;

		for (int i=0;i<3;i++)
		{
			float z = pFace->getVertexPointer(i)->z;
			mesh.setColor(pFace->getVertexIndex(i), ofColor( ofMap(z,-40,24, 20,180) ));
		}
	}
*/

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
string apparelModel::toString()
{
	string s = "";
	s = "\tid = "+id+"\n";
	s += "\tnb mesh vertices = "+ofToString(mesh.getVertices().size())+"\n";
	s += "\tnb mesh indices = "+ofToString(mesh.getIndices().size())+"\n";
	s += "\tnb mesh face = "+ofToString(meshFaces.size())+"\n";
	
	#if APPARELMODEL_TOSTRING_LISTS
	int nbVertices = (int) mesh.getVertices().size();
	for (int i=0; i<nbVertices; i++ )
	{
		ofVec3f v = mesh.getVertices()[i];
		s += "\t["+ofToString(i)+"] ("+ofToString(v.x)+";"+ofToString(v.y)+";"+ofToString(v.y)+")\n";
		
	}

	#endif
	
	return s;
}

//--------------------------------------------------------------
void apparelModel::createMeshFaces()
{
//	OFAPPLOG->begin("apparelModel::createMeshFaces()");
	clearMeshFaces();

	vector<ofIndexType>& 	indices 	= mesh.getIndices();

	for (int i=0; i<indices.size();i+=3)
	{
		meshFaces.push_back(  new ofMeshFaceApparel(&mesh, i) );
	}
	
//	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelModel::computeMeshFacesNormals()
{
	vector<ofMeshFaceApparel*>::iterator it = meshFaces.begin();
	for ( ; it!=meshFaces.end(); ++it)
	{
		(*it)->getFaceNormal();
	}
}

//--------------------------------------------------------------
void apparelModel::removeMeshFace(int index)
{
/*
	if (index < meshFaces.size())
	{
		ofMeshFace 			faceToRemove = meshFaces[index];
		ofMeshFaceIndices	faceIndicesToRemove = meshFacesIndices[index];
	
		// Remove indices from mesh
		mesh.removeIndices( faceIndicesToRemove.m_index[0], faceIndicesToRemove.m_index[0]+3);

		// Remove instances from list
	 	meshFacesIndices.erase(meshFacesIndices.begin()+index);
	 	meshFaces.erase(meshFaces.begin()+index);
		
		createMeshFaces();
	}
	else
	{
		OFAPPLOG->begin("apparelModel::removeMeshFace(\""+ofToString(index)+"\")");
		OFAPPLOG->println(ofToString(index)+" is not a valid index (max is "+ofToString(meshFaces.size()-1)+")");
		OFAPPLOG->end();
	}
*/
	
}

//--------------------------------------------------------------
void apparelModel::clearMeshFaces()
{
	vector<ofMeshFaceApparel*>::iterator it = meshFaces.begin();
	for ( ; it != meshFaces.end(); ++it )
	{
		delete *it;
	}
	meshFaces.clear();
}


//--------------------------------------------------------------
void apparelModel::removeMeshFaces(const vector<int>& listFaces)
{
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
    modelMatrix.glScale(scale.x, scale.y, scale.z);
}


//--------------------------------------------------------------
void apparelModel::copyMeshAndTransformation(const apparelModel& otherModel)
{
	// OFAPPLOG->begin("apparelModel::copyMeshAndTransformation()");

	id 		= otherModel.id;
	mesh 	= otherModel.mesh;
	pos 	= otherModel.pos;
	scale 	= otherModel.scale;

	createMeshFaces();
	updateModelMatrix();

	// OFAPPLOG->println(" - mesh (copy) has "+ofToString(mesh.getNumVertices())+" vertices & "+ofToString(mesh.getNumIndices())+" indices");
	//OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelModel::copyVerticesPositions(const apparelModel& otherModel)
{
	// ofMesh& otherMesh = otherModel.getMeshRef();
	
}

//--------------------------------------------------------------
void apparelModel::computeFacesNormals()
{
	vector<ofMeshFaceApparel*>::iterator it = meshFaces.begin();
	for ( ; it != meshFaces.end(); ++it )
	{
//		(*it)->
	}
}






