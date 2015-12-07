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
#else
	#include "oscSender.h"
#endif

#define APPAREL_MOD_DEBUG_LOG 1

//--------------------------------------------------------------

//--------------------------------------------------------------
apparelMod::apparelMod(string id, int id2)
{
	m_id 					= id;
	m_id2					= id2;
	mp_oscSender 			= 0;
	m_isChanged				= true;
	m_isMeshChanged			= false;
	m_isMood				= false;
	m_weight				= 0.25f;
	m_bForceWeightAutomatic	= false;


//	m_weight.set("Weight", 0.5f, 0.0f, 1.0f);
//	m_parameters.add(m_weight);
//	createParameters();
	
	m_parameters.setName(id);
	
	m_flagChanged	= 0;
}

//--------------------------------------------------------------
void apparelMod::createParameters()
{
   m_isWeightManual.set("WeightManual",false);
   m_parameters.add(m_isWeightManual);

   m_nbWordsMax.set("NbWordsMax", 5, 1, 20);
   m_parameters.add(m_nbWordsMax);
}


//--------------------------------------------------------------
void apparelMod::onNewWords(user* pUser, vector<string>& wordsMessage)
{
 // OFAPPLOG->begin("apparelMod["+m_id+"]::onNewWords()");
  if (pUser && pUser->getSqlData())
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
  else
  {
	  OFAPPLOG->println("- ERROR, user is not set or user sql data base not opened ...");
  }

 // OFAPPLOG->end();
}

//--------------------------------------------------------------
bool apparelMod::isInWordsList(string word)
{
/*	int nbWordsInList = m_words.size();
	for (int i=0;i<nbWordsInList;i++)
	{
		if (word == m_words[i])
		{
			return true;
		}
	}
	return false;
*/
	return  (find(m_words.begin(), m_words.end(), word) != m_words.end());
}

//--------------------------------------------------------------
void apparelMod::updateUserDatabase(user* pUser, string word, bool lock)
{
	OFAPPLOG->begin("apparelMod::resetWordsCountUserDatabase()");

	if(pUser && pUser->getSqlData())
	{
	  if (lock)
		  pUser->lock();
	
	  ofxSQLiteSelect sel = pUser->getSqlData()->select("count").from("words").where("name", word).limit(1).execute().begin();

	  // Found it !
	  if (sel.hasNext())
	  {
		int count = sel.getInt();
		OFAPPLOG->println("- found word '"+word+"' with count="+ofToString(count));

		// update count
		count = count+1;
		int result = pUser->getSqlData()->update("words").where("name",word).use("count", count).execute();
		if (result == 0)
		{
			OFAPPLOG->println("- OK updated word '"+word+"' with count="+ofToString(count));
		}
	  }
	  // No ? Insert it with a count of one
	  else
	  {
		int result = pUser->getSqlData()->insert("words").use("name", word).use("count", 1).use("mod", m_id2).execute();
		if (result == 0)
		{
			OFAPPLOG->println("- OK inserted word '"+word+"'");
		}
	  }
	 
	  if (lock)
	   	pUser->unlock();

	}
	else
	{
	 	OFAPPLOG->println("- ERROR, user is not set or user sql data base not opened ...");
	}

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod::resetWordsCountUserDatabase(user* pUser, bool lock)
{
	OFAPPLOG->begin("apparelMod::resetWordsCountUserDatabase()");
	if (pUser && pUser->getSqlData())
	{
		if (lock)
			pUser->lock();
		int nbWords = m_words.size();
		for (int i=0;i<nbWords;i++)
		{
			ofxSQLiteSelect sel = pUser->getSqlData()->select("count").from("words").where("name", m_words[i]).limit(1).execute().begin();
			// Found it !
			if (sel.hasNext())
			{
				int result = pUser->getSqlData()->update("words").where("name",m_words[i]).use("count", 0).execute();
			  	if (result == 0)
			  	{
		  			OFAPPLOG->println("- OK updated word "+m_words[i]+" with count=0");
			  	}
			}
		}

		countUserWords(pUser);
		if (lock)
			pUser->unlock();

		if (mp_oscSender)
		{
			#ifndef TARGET_OF_IOS
	   		((oscSender*)mp_oscSender)->sendModEmptyUserDataSQL(this);
			#endif
		}

	 }
	 else
	 {
	 	OFAPPLOG->println("- ERROR, user is not set or user sql data base not opened ...");
	 }
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void apparelMod::countUserWords(user* pUser, bool lock)
{
  if (isMood()) return;

 // OFAPPLOG->begin("apparelMod["+m_id+"]::countUserWords()");
  if (pUser)
  {
	m_countWords = 0;
  	if (pUser->getSqlData())
  	{
		if (lock)
			pUser->lock();
		int nbWordsInList = m_words.size();
/*
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
*/
		ofxSQLiteSelect sel = pUser->getSqlData()->select("sum(count) AS total_count").from("words").where("mod", m_id2).limit(1).execute().begin();
		//SELECT sum(count) AS total_count FROM words WHERE mod = 0;
		if (sel.hasNext())
		{
			m_countWords = sel.getInt();
		  	OFAPPLOG->println("- m_countWords ="+ofToString(m_countWords));
		}


		if (lock)
			pUser->unlock();

	    // Update weight if we are not manual
	    if (m_isWeightManual == false)
  		{
	 		if (m_nbWordsMax>0)
			   setWeight( (float) m_countWords / (float) m_nbWordsMax );
  			else
	   			setWeight( 0.0 );
  		}
		  //OFAPPLOG->println("- countWords="+ofToString(m_countWords)+"/m_nbWordsMax="+ofToString(m_nbWordsMax));
		  //OFAPPLOG->println("- weight="+ofToString(m_weight));
  	}
  	else
	  OFAPPLOG->println("- WARNING pUser->getSqlData() is NULL");
	}
  	else
	  OFAPPLOG->println("- WARNING pUser is NULL");

//  OFAPPLOG->end();
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
	// ofLog() << "parameterChanged / name=" << parameter.getName() << " type= "<< typeid(parameter).name();

	// Particular treatment for weightManual
	if (parameter.getName() == "WeightManual")
	{
		if (m_isWeightManual)
		{
			setWeight(m_weight); // manually
		}
		else
		{
			countUserWords( GLOBALS->getUser(),true ); // from sql database
		}
		setChanged();
	}
	else
	if (parameter.getName() == "NbWordsMax")
	{
		countUserWords( GLOBALS->getUser(),true ); // from sql database
	}
	else
		onParameterChanged(parameter);

	// Send to OSC if instance is setup
	if (mp_oscSender){
		mp_oscSender->sendParameter( parameter );
	}
}

//--------------------------------------------------------------
void apparelMod::setWeight(float v)
{
	m_weight = v;
    if (m_weight>1.0f) m_weight = 1.0f;
	onWeightChanged();
	setChanged();

	if (mp_oscSender){
		mp_oscSender->sendModWeight( this );
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
/*		for (int i=0; i<m_words.size(); i++){
			strWords+=delimiter + m_words[i];
			delimiter=" / ";
		}
*/
		OFAPPLOG->println("- found "+ofToString(m_words.size())+" words : " + strWords);
		
	}
	else{
		OFAPPLOG->println("- cannot find 'wordlist.txt'");
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
	
	if (m_bForceWeightAutomatic)
	{
		m_isWeightManual = false;
	}


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
	clearSelectionVertices();
	clearSelectionFaces();
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::clearSelectionVertices()
{
	m_indicesVertex.clear();
	m_model.mesh = m_meshInput;
	setChanged(true);
}

//--------------------------------------------------------------
void apparelMod::clearSelectionFaces()
{
	m_indicesFaces.clear();
	m_model.mesh = m_meshInput;
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








