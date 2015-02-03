//
//  apparelMod.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#include "apparelMod.h"
#include "globals.h"
#ifdef TARGET_OF_IOS
#include "ofxIOSExtras.h"
#endif

//--------------------------------------------------------------

//--------------------------------------------------------------
apparelMod::apparelMod(string id)
{
	m_id 					= id;
	mp_oscSender 			= 0;
	m_isChanged				= false;
	m_isMeshChanged			= false;
	
//	m_isActive.setName("Active");
//	m_parameters.add(m_isActive);

	m_nbWordsMax.set("NbWordsMax", 5, 1, 20);
	m_parameters.add(m_nbWordsMax);

	m_weight.set("Weight", 0.5f, 0.0f, 1.0f);
	m_parameters.add(m_weight);


	m_parameters.setName(id);
	
	m_flagChanged	= 0;
}

//--------------------------------------------------------------
void apparelMod::onNewWords(user* pUser, vector<string>& wordsMessage)
{
  OFAPPLOG->begin("apparelMod["+m_id+"]::onNewWords()");
  if (pUser->getSqlData())
  {
	   // For every word of the wordsList.txt
		int nbWordsInMessage = wordsMessage.size();
		for (int i=0;i<nbWordsInMessage;i++)
		{
			if (isInWordsList(wordsMessage[i]))
			{
				updateUserDatabase(pUser,wordsMessage[i]);
			}
		}
		
		// Update user words
		countUserWords(pUser);
  }
  OFAPPLOG->end();
}

//--------------------------------------------------------------
bool apparelMod::isInWordsList(string word)
{
	int nbWordsInList = m_words.size();
	for (int i=0;i<nbWordsInList;i++)
	{
		if (word == m_words[i])
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void apparelMod::updateUserDatabase(user* pUser, string word)
{
	ofxSQLiteSelect sel = pUser->getSqlData()->select("count").from("words").where("name", word).limit(1).execute().begin();

	// Found it !
	if (sel.hasNext())
	{
	  int count = sel.getInt();
	  OFAPPLOG->println("- found word "+word+" with count="+ofToString(count));

	  // update count
	  count = count+1;
	  int result = pUser->getSqlData()->update("words").where("name",word).use("count", count).execute();
	  if (result == 0)
	  {
		  OFAPPLOG->println("- OK updated word "+word+" with count="+ofToString(count));
	  }
	}
	// No ? Insert it with a count of one
	else
	{
	  int result = pUser->getSqlData()->insert("words").use("name", word).use("count", 1).execute();
	  if (result == 0)
	  {
		  OFAPPLOG->println("- OK inserted word "+word);
	  }
	}
}

//--------------------------------------------------------------
void apparelMod::countUserWords(user* pUser)
{
  OFAPPLOG->begin("apparelMod["+m_id+"]::countUserWords()");
  m_countWords = 0;
  if (pUser->getSqlData())
  {
	int nbWordsInList = m_words.size();
	for (int i=0;i<nbWordsInList;i++)
	{
		ofxSQLiteSelect sel = pUser->getSqlData()->select("count").from("words").where("name", m_words[i]).limit(1).execute().begin();
		if (sel.hasNext())
		{
		  int count = sel.getInt();
		  OFAPPLOG->println("- found word "+m_words[i]+" with count="+ofToString(count));
		  m_countWords += count;
		}
	}
  }
  else
	  OFAPPLOG->println("- WARNING pUser->getSqlData() is NULL");

  OFAPPLOG->println("- countWords="+ofToString(m_countWords));

  // Update weight now and crop
  m_weight = (float) m_countWords / (float) m_nbWordsMax;
  if (m_weight>1.0f)
		m_weight = 1.0f;

  // Mod changed, update it
  setChanged();

  OFAPPLOG->println("- weight="+ofToString(m_weight.get()));
  OFAPPLOG->end();
}


//--------------------------------------------------------------
ofAbstractParameter& apparelMod::getParameter(string name)
{
	return m_parameters.get(name);
}

//--------------------------------------------------------------
void apparelMod::addFaceIndex(int faceIndex)
{
	m_indicesFaces.push_back(faceIndex);
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::removeFaceIndex(int faceIndex)
{
	 m_indicesFaces.erase(std::remove(m_indicesFaces.begin(), m_indicesFaces.end(), faceIndex), m_indicesFaces.end());
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::addVertexIndex(int vertexIndex)
{
	m_indicesVertex.push_back(vertexIndex);
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::removeVertexIndex(int vertexIndex)
{
	 m_indicesVertex.erase(std::remove(m_indicesVertex.begin(), m_indicesVertex.end(), vertexIndex), m_indicesVertex.end());
	setChanged(true);
}


//--------------------------------------------------------------
void apparelMod::parameterChanged(ofAbstractParameter & parameter)
{
	onParameterChanged(parameter);
	if (mp_oscSender){
		mp_oscSender->sendParameter( parameter );
	}
}

//--------------------------------------------------------------
string apparelMod::getPathRelative(string filename)
{
	string path = "mods/"+m_id+"/";
	if (!filename.empty())
	{
		path += filename;
	}
	return path;
}

//--------------------------------------------------------------
string apparelMod::getPathDocument(string filename)
{
	#ifdef TARGET_OF_IOS
		return ofxiOSGetDocumentsDirectory() + getPathRelative(filename);
	#else
		return ofToDataPath(getPathRelative(filename));
	#endif
}


//--------------------------------------------------------------
string apparelMod::getPathResources(string filename)
{
	return ofToDataPath(getPathRelative(filename));
}

//--------------------------------------------------------------
void apparelMod::createWordsList()
{
	OFAPPLOG->begin("apparelMod::createWordsList() for instance \"" + m_id + "\"");
	
	string pathWordsList = getPathResources("wordlist.txt");
	ofFile fWordsList( pathWordsList );
	if (fWordsList.exists())
	{
		OFAPPLOG->println("- loaded 'wordlist.txt'");

		ofBuffer buffer = ofBufferFromFile(pathWordsList, false);
		m_words = ofSplitString(buffer.getText(), ",", true, true);

		string strWords="";
		string delimiter="";
		for (int i=0; i<m_words.size(); i++){
			strWords+=delimiter + m_words[i];
			delimiter=" / ";
		}

		OFAPPLOG->println("- found "+ofToString(m_words.size())+" words : " + strWords);
		
	}
	else{
		OFAPPLOG->begin("- cannot find 'wordlist.txt'");
	}


	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod::createDirMod()
{
	ofDirectory dirMod( getPathDocument() );

	if (!dirMod.exists()){
		dirMod.create(true);
	}
}



//--------------------------------------------------------------
void apparelMod::readModel()
{
   OFAPPLOG->begin("apparelMod::readModel()");


   m_indicesFaces.clear();
   m_indicesVertex.clear();
   
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

  OFAPPLOG->println("- nb faces="+ofToString( m_indicesFaces.size() ));
  OFAPPLOG->println("- nb vertices="+ofToString( m_indicesVertex.size() ));
 
 
  OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod::loadModel()
{
	OFAPPLOG->begin("apparelMod::loadModel() for instance \"" + m_id + "\"");
	// MODEL DATA
	string pathModel = getPathResources("model.xml");
	if ( m_settingsModel.load(pathModel) )
	{
		OFAPPLOG->println("- loaded "+pathModel);
		readModel();
	}
	else{
		OFAPPLOG->println(OF_LOG_ERROR, "- error loading "+pathModel);
	}

	OFAPPLOG->end();
	
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::loadModel(string& xml)
{
	m_settingsModel.loadFromBuffer(xml);
	readModel();
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::saveModel()
{
	OFAPPLOG->begin("apparelMod::saveModel() for instance \"" + m_id + "\"");
	createDirMod();

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

	string pathModel = getPathResources("model.xml");

	OFAPPLOG->println("- saving " + pathModel );
	settings.save(pathModel);
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
string apparelMod::getModelAsXMLString()
{
	string s="";
	
	ofxXmlSettings settings;
	if (settings.load(getPathResources("model.xml")))
	{
		settings.copyXmlToString(s);
	}
	
	return s;
}


//--------------------------------------------------------------
void apparelMod::loadParameters()
{
	OFAPPLOG->begin("apparelMod::loadParameters() for instance \"" + m_id + "\"");

	// WORDS
	createWordsList();

	// Add listener for PARAMETERS
	ofAddListener(m_parameters.parameterChangedE, this, &apparelMod::parameterChanged);

	// PARAMETERS DATA
	ofxXmlSettings settingsParameters;
	string pathParameters = getPathDocument("parameters.xml");

	#ifdef TARGET_OF_IOS
	string pathSettingsParametersSource = getPathResources("parameters.xml");
	ofFile fileSettingsParametersSource( pathSettingsParametersSource );
	if (fileSettingsParametersSource.exists())
	{
		OFAPPLOG->println("- copying from "+pathSettingsParametersSource+" to "+pathParameters);
		bool result = fileSettingsParametersSource.copyTo(pathParameters);
		OFAPPLOG->println("- copy OK ? " + ofToString(result));
 	}

	#endif

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


	OFAPPLOG->end();
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::saveParameters()
{
	OFAPPLOG->begin("apparelMod::saveParameters() for instance \"" + m_id + "\"");
	createDirMod();

	// PARAMETERS DATA
	ofxXmlSettings settings;
	settings.serialize(m_parameters);
	string pathParameters = getPathDocument("parameters.xml");

	OFAPPLOG->println("- saving " + pathParameters );
	settings.save(pathParameters);

	OFAPPLOG->end();
}


//--------------------------------------------------------------
void apparelMod::setConfiguration(string name)
{
	m_configurationName = name;
}

//--------------------------------------------------------------
void apparelMod::copyModelFrom(const apparelModel& model)
{
	m_model.copyMeshAndTransformation(model);
	m_meshInput = model.mesh;
//	clearSelection();
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::copyModelVerticesPosition(const apparelModel& model)
{
	m_model.copyVerticesPositions(model);
	
	setChanged(true);
}


//--------------------------------------------------------------
void apparelMod::clearSelection()
{
	m_indicesVertex.clear();
	m_indicesFaces.clear();
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::drawFaces()
{
	m_model.drawFaces();
}

//--------------------------------------------------------------
void apparelMod::drawWireframe()
{
	m_model.drawWireframe();
}








