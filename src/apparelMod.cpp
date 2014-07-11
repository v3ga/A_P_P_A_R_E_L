//
//  apparelMod.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#include "apparelMod.h"
#include "globals.h"

//--------------------------------------------------------------
apparelMod::apparelMod(string id)
{
	m_id 			= id;
	mp_model 		= 0;
	mp_oscSender 	= 0;
}

//--------------------------------------------------------------
void apparelMod::addFaceIndex(int faceIndex)
{
	m_indicesFaces.push_back(faceIndex);
}

//--------------------------------------------------------------
void apparelMod::removeFaceIndex(int faceIndex)
{
	 m_indicesFaces.erase(std::remove(m_indicesFaces.begin(), m_indicesFaces.end(), faceIndex), m_indicesFaces.end());
}


//--------------------------------------------------------------
void apparelMod::parameterChanged(ofAbstractParameter & parameter)
{
	// ofLog() << "apparelMod::parameterChanged() " << parameter.getName();
	if (mp_oscSender){
		mp_oscSender->sendParameter( parameter );
	}
}

//--------------------------------------------------------------
void apparelMod::loadParameters()
{
	if ( m_settingsModel.load(getPathMod("model.xml")) )
	{
		m_indicesFaces.clear();
		
		m_settingsModel.pushTag("mod");
		m_settingsModel.pushTag("faces");
		int nbFaces = m_settingsModel.getNumTags("index");
		for (int i=0; i<nbFaces; i++)
		{
			m_indicesFaces.push_back( m_settingsModel.getValue("index",0,i) );
		}
		m_settingsModel.popTag();
		m_settingsModel.popTag();
	}
	
	// Custom loading
	loadParametersCustom();

	// Add listener
	ofAddListener(m_parameters.parameterChangedE, this, &apparelMod::parameterChanged);
}

//--------------------------------------------------------------
void apparelMod::saveParameters()
{
	// Faces
	ofxXmlSettings settings;
	
	settings.addTag("mod");
	settings.pushTag("mod");

	settings.addTag("faces");
	settings.pushTag("faces");
	vector<int>::iterator facesIt;
	for (facesIt = m_indicesFaces.begin(); facesIt != m_indicesFaces.end(); ++facesIt)
	{
		settings.addValue("index", *facesIt);
	}
	settings.popTag();

	// Vertices
	settings.addTag("vertices");
	settings.pushTag("vertices");
	vector<int>::iterator verticesIt;
	for (verticesIt = m_indicesVertex.begin(); verticesIt != m_indicesVertex.end(); ++verticesIt)
	{
		settings.addValue("index", *verticesIt);
	}
	settings.popTag();

	settings.popTag();


	// Save
	ofLog() << "saving " << getPathMod("model.xml");
	settings.save(getPathMod("model.xml"));
}





