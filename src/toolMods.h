//
//  toolMods.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#pragma once

#include "tool.h"
#include "apparelMod.h"
#include "apparelModUI.h"
#include "meshFaceSelector.h"
#include "meshVertexSelector.h"

class apparelModel;
class apparelModManager;
class toolMods : public tool
{
	public:
		toolMods				(toolManager* parent, apparelModManager* modManager);
		~toolMods				();

		void					update				();
		void					draw				();

		void					show				(bool is);
		bool					isHit				(int x, int y);
		void					exit				();
	
		void					createControlsCustom();
		void					createControlsCustomFinalize();
		void					handleEvents		(ofxUIEventArgs& e);
		void					mousePressed		(int x, int y, int button);
		apparelModUI*			makeInstanceModUI	(apparelMod*);
		void					selectMod			(string name);
		void					selectMood			(string name);
	
		// Manager
		apparelModManager*		mp_modsManager;

		// UI
		apparelModUI*				mp_modUICurrent;
		map<string, apparelModUI*>	m_modsUI;

		apparelModUI*				mp_moodUICurrent;
		map<string, apparelModUI*>	m_moodsUI;

		// View
		bool					m_bViewMixed;
 
		// Post Processing
		bool					m_bPostProcess;
		bool					isPostProcessEnabled(){return m_bPostProcess;}


		// Selection 3D
		apparelMod*				mp_apparelModCurrent;
	
		apparelModel*			mp_apparelModel;
		meshFaceSelector		m_meshFaceSelector;
		meshVertexSelector		m_meshVertexSelector;
		bool					m_bShowVertexNormals;
		bool					m_bShowVertexIndices;
		bool					m_bEnableBackFaceCulling;
		bool					m_bDrawWireFrameOnly;

		bool					m_bShowFaceNormals;
		bool					m_bShowFaceIndices;
		ofMeshFaceApparel*		mp_meshFaceOver;
		int						m_meshFaceIndexOver;

		ofVec3f*				mp_meshVertexOver;
		ofVec2f					m_meshVertexScreenOver;
		int						m_meshVertexIndexOver;
	
		ofSpherePrimitive				m_sphereVertexOver;
		map<int, ofSpherePrimitive*>	m_mapSphereVertexSelected;
	
	
		bool					isFaceSelected		(int index);
		bool					isVertexSelected	(int index);
	
		ofColor					colorFaceOver;
		ofColor					colorFaceSelected;
	

		enum enumSelection
		{
			E_selection_vertex 	= 0,
			E_selection_face 	= 1
		};
	
		enumSelection			m_selection;
		void					unselectSelection	();
 
	private:
		ofxUILabel*				mp_lblModel;
		bool					isNameMod			(string id);
		bool					isNameMood			(string id);
		void					drawVertexNormals	(apparelModel* pModel);
		void					drawFaceNormals		(apparelModel* pModel);
		void					drawSelection		(apparelModel* pModel);

};
