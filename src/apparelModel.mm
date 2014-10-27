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
	ofxXmlSettings properties;
	
	properties.addTag("properties");
	properties.pushTag("properties");
		properties.addTag("position");
		properties.pushTag("position");
			properties.addValue("x", getPosition().x);
			properties.addValue("y", getPosition().y);
			properties.addValue("z", getPosition().z);
		properties.popTag();

		properties.addValue("scale", getScale().x);

	properties.popTag();

	return properties.save( getPathRelative(getPropertiesFilename()) );

}

//--------------------------------------------------------------
string apparelModel::getPathRelative(string filename)
{
	string path = "3d/"+filename;
	if (!filename.empty())
	{
		path += filename;
	}
	return path;
}

//--------------------------------------------------------------
string apparelModel::getPropertiesFilename()
{
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

