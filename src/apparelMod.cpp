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

//--------------------------------------------------------------
apparelMod::apparelMod(string id)
{
	m_id 			= id;
	mp_model 		= 0;
	mp_oscSender 	= 0;
	
	m_isActive.setName("Active");
	m_parameters.add(m_isActive);

	m_weight.set("Weight", 0.5f, 0.0f, 1.0f);
	m_parameters.add(m_weight);

	m_parameters.setName(id);
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
void apparelMod::addVertexIndex(int vertexIndex)
{
	m_indicesVertex.push_back(vertexIndex);
}

//--------------------------------------------------------------
void apparelMod::removeVertexIndex(int vertexIndex)
{
	 m_indicesVertex.erase(std::remove(m_indicesVertex.begin(), m_indicesVertex.end(), vertexIndex), m_indicesVertex.end());
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
string apparelMod::getPathMod(string filename)
{
	string path = "mods/"+m_id+"/";
	if (!filename.empty())
	{
		path += filename;
	}
	return path;
}




//--------------------------------------------------------------
void apparelMod::loadParameters()
{
	OFAPPLOG->begin("apparelMod::loadParameters() for instance \"" + m_id + "\"");

	// MODEL DATA
	string pathModel = getPathMod("model.xml");
	if ( m_settingsModel.load(pathModel) )
	{
		OFAPPLOG->println("- loaded "+pathModel);

		m_indicesFaces.clear();
		
		m_settingsModel.pushTag("mod");

			m_settingsModel.pushTag("faces");
			int nbFaces = m_settingsModel.getNumTags("index");
			for (int i=0; i<nbFaces; i++)
			{
				m_indicesFaces.push_back( m_settingsModel.getValue("index",0,i) );
			}
			m_settingsModel.popTag();

			m_settingsModel.pushTag("vertices");
			int nbVertices = m_settingsModel.getNumTags("index");
			for (int i=0; i<nbVertices; i++)
			{
				m_indicesVertex.push_back( m_settingsModel.getValue("index",0,i) );
			}
			m_settingsModel.popTag();
		
			
	   m_settingsModel.popTag();
	}
	else{
		OFAPPLOG->println(OF_LOG_ERROR, "- error loading "+pathModel);
	}

	// PARAMETERS CONFIGURATION DATA
/*	if (sm_isConfigurationsLoaded == false)
	{
		string pathConfigurations = getPathToolMods("mods_configurations.xml");
		if (sm_settingsConfigurations.load(pathConfigurations))
		{
			OFAPPLOG->println("- loaded "+pathConfigurations);
		}
		else{
			OFAPPLOG->println(OF_LOG_ERROR, "- error loading "+pathConfigurations);
		}

		sm_isConfigurationsLoaded = true;
	}
*/


	// PARAMETERS DATA
	ofxXmlSettings settingsParameters;
	string pathParameters = getPathMod("parameters.xml");
	if (settingsParameters.load(pathParameters))
	{
		settingsParameters.deserialize(m_parameters);
		OFAPPLOG->println("- loaded "+pathParameters);
	}
	else{
		OFAPPLOG->println(OF_LOG_ERROR, "- error loading "+pathParameters);
	}

	// Custom loading
	loadParametersCustom();

	// Add listener
	ofAddListener(m_parameters.parameterChangedE, this, &apparelMod::parameterChanged);

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod::saveParameters()
{
	// CREATE PATH FIRST
	ofDirectory dirMod( getPathMod() );
	if (!dirMod.exists())
	{
		dirMod.create();
	}

	// MODEL DATA
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

	// PARAMETERS DATA
	ofxXmlSettings settings2;
	settings2.serialize(m_parameters);

	
	// Save
	OFAPPLOG->println("- saving " + getPathMod("model.xml") );
	settings.save(getPathMod("model.xml"));

	OFAPPLOG->println("- saving " + getPathMod("parameters.xml") );
	settings2.save(getPathMod("parameters.xml"));
}


//--------------------------------------------------------------
void apparelMod::setConfiguration(string name)
{
	m_configurationName = name;
}





