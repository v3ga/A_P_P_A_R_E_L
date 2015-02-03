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

class apparelMod
{
	public:
		apparelMod			(string id);
		virtual ~apparelMod	(){}
	
	
				void		createDirMod		();

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

/*		enum{
			EChanged_selectionVertices	= 0x01,
			EChanged_selectionFaces		= 0x02,
			EChanged_numberVertices		= 0x04,
			EChanged_numberFaces		= 0x08
		};
*/

		// DRAWING
		virtual void		draw					(){};
		virtual void		drawExtra				(){};
		virtual	void		drawFaces				();
		virtual	void		drawWireframe			();

		// OSC NETWORK
		void				setOscSender			(oscSenderInterface* p){mp_oscSender=p;}

		// INTERFACE FOR RECEIVING DATA FROM USER
		void				onNewText				(string text){}
		void				onNewWords				(vector<string>& text){}

		// INTERFACE FOR SAVING IN DATABASE
	
		// ID
		string				m_id;
		string				getId					(){return m_id;}

		// MODEL w/ DATA
		apparelModel		m_model;
	   	vector<int>			m_indicesFaces;		// selected faces on original model
		vector<int>			m_indicesVertex;	// selected vertices on original model

		virtual void		copyModelFrom				(const apparelModel& model);
		virtual	void		copyModelVerticesPosition	(const apparelModel& model);
		virtual void		clearSelection				();
		virtual void		apply						(){};
 
		// UPDATE
		virtual	void		update				(){}

		// PARAMETERS CONFIGURATION
		string				m_configurationName;
	
		// PARAMETERS
		ofParameterGroup 	m_parameters;
		ofParameter<bool>	m_isActive; // DEPRECATED
		ofParameter<float>	m_weight;


		// SETTINGS
		ofxXmlSettings		m_settingsModel;
	
		// WORDS
		vector<string>		m_words;
		//ofBuffer buffer = ofBufferFromFile("someFile.txt");


		// OSC
		oscSenderInterface*	mp_oscSender;
};
