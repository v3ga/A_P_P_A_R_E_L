//
//  apparelMod.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 27/05/2014.
//
//

#pragma once
#include "ofMain.h"
#include "apparelModel.h"
#include "ofxXmlSettings.h"
#include "oscSenderInterface.h"
#include "user.h"

class apparelMod
{
	public:
		apparelMod			(string id, int id2=0);
		virtual ~apparelMod	(){}
	
				void		setId				(int id){m_id2 = id;}
	
				void		createDirMod		();

		virtual	void		createParameters	();
		virtual	void		loadParameters		();
		virtual	void		loadParametersCustom(){};
		virtual	void		saveParameters		();

		virtual	void		readModel			();
		virtual	void		loadModel			();
		virtual	void		loadModel			(string& xml);
		virtual	void		saveModel			();
		virtual	string		getModelAsXMLString	();
	
		void				setConfiguration	(string name);

		string				getPathDocument		(string filename="");
		string				getPathResources	(string filename="");

		void				createWordsList		();

	
		ofParameterGroup&	getParamatersGroupRef	(){return m_parameters;}
		ofAbstractParameter&getParameter			(string name);
		virtual void		parameterChanged		(ofAbstractParameter & parameter);
		virtual	void		onParameterChanged		(ofAbstractParameter & parameter){}
		virtual	void		onWeightChanged			(){}
		void				setForceWeightAutomatic	(bool is=true){m_bForceWeightAutomatic = is;}

		string				getPathRelative			(string filename="");
		string				getPathToolMods			(string filename){return "tools/mods/"+filename;}

		void				addFaceIndex			(int faceIndex);
		void				removeFaceIndex			(int faceIndex);
	
		void				addVertexIndex			(int vertexIndex);
		void				removeVertexIndex		(int vertexIndex);


		// STATE
		ofMesh				m_meshInput;


		void				setChanged				(bool is=true){m_isChanged = is;}
		void				setMeshChanged			(bool is=true){m_isMeshChanged = is;}

		bool				isChanged				(){return m_isChanged;}
		bool				isMeshChanged			(){return m_isMeshChanged;}

 		bool				m_isChanged;			// tells if mesh itself or selection (vertices, faces) changed
 		bool				m_isMeshChanged;		// tells if mesh itself or selection (vertices, faces) changed
		int					m_flagChanged;


		// MOOD
		bool				m_isMood;
		bool				isMood					(){return m_isMood;}

		// DRAWING
		virtual void		draw					(){};
		virtual void		drawExtra				(){};
		virtual	void		drawFaces				();
		virtual	void		drawWireframe			();

		// OSC NETWORK
		void				setOscSender			(oscSenderInterface* p){mp_oscSender=p;}

		// INTERFACE FOR RECEIVING DATA FROM USER : !!can be asynchronous!!
		void				onNewText				(user* pUser, string text){}
		void				onNewWords				(user* pUser, vector<string>& words);

		// INTERFACE FOR SAVING IN DATABASE
	
		// ID
		string				m_id;
		string				getId					(){return m_id;}
 
		int					m_id2;

		// MODEL w/ DATA
		apparelModel		m_model;
	   	vector<int>			m_indicesFaces;		// selected faces on original model
		vector<int>			m_indicesVertex;	// selected vertices on original model

		virtual void		copyModelFrom				(const apparelModel& model);
		virtual	void		copyModelVerticesPosition	(const apparelModel& model);
		virtual void		clearSelection				();
		virtual	void		clearSelectionVertices		();
		virtual	void		clearSelectionFaces			();
		virtual void		apply						(){};
 
		// UPDATE
		virtual	void		update						(){}

		// PARAMETERS CONFIGURATION
		string				m_configurationName;
	
		// PARAMETERS
		ofParameterGroup 	m_parameters;
		ofParameter<bool>	m_isWeightManual;
		ofParameter<int>	m_nbWordsMax;

	    bool				m_bForceWeightAutomatic; // if true, overwrites m_isWeightManual when loadParameters is called

		float				m_weight;
		void				setWeight					(float v);


//		bool				m_isWeightManual;	// if true, user can modify manually the value otherwise it is computed by


		// SETTINGS
		ofxXmlSettings		m_settingsModel;
	
		// WORDS
		vector<string>		m_words;
		int					m_countWords; // total words from list retrieved in database
 
		bool				isInWordsList				(string word);
		void				countUserWords				(user* pUser, bool lock=false);
		void				updateUserDatabase			(user* pUser, string word, bool lock=false);
		void				emptyUserDatabase			(user* pUser, bool lock=false);
 		void				resetWordsCountUserDatabase	(user* pUser, bool lock=false);
 
		// OSC
		oscSenderInterface*	mp_oscSender;
};
