//
//  apparelMod.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#include "apparelMod.h"

//--------------------------------------------------------------
apparelMod::apparelMod(string id)
{
	m_id = id;
	mp_model = 0;
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
void apparelMod::loadParameters()
{
	if ( m_settingsModel.load(getPathMod("model.xml")) )
	{
		m_indicesFaces.clear();
		
		m_settingsModel.pushTag("faces");
		int nbFaces = m_settingsModel.getNumTags("index");
		for (int i=0; i<nbFaces; i++)
		{
			m_indicesFaces.push_back( m_settingsModel.getValue("index",0,i) );
		}
		m_settingsModel.popTag();
	}
}

//--------------------------------------------------------------
void apparelMod::saveParameters()
{
	// Faces
	m_settingsModel.addTag("faces");
	m_settingsModel.pushTag("faces");
	vector<int>::iterator facesIt;
	for (facesIt = m_indicesFaces.begin(); facesIt != m_indicesFaces.end(); ++facesIt)
	{
		m_settingsModel.addValue("index", *facesIt);
	}
	m_settingsModel.popTag();

	// Vertices
	m_settingsModel.addTag("vertices");
	m_settingsModel.pushTag("vertices");
	vector<int>::iterator verticesIt;
	for (verticesIt = m_indicesVertex.begin(); verticesIt != m_indicesVertex.end(); ++verticesIt)
	{
		m_settingsModel.addValue("index", *verticesIt);
	}
	m_settingsModel.popTag();


	// Save
	ofLog() << "saving " << getPathMod("model.xml");
	m_settingsModel.save(getPathMod("model.xml"));
}





