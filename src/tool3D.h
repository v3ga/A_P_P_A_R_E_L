//
//  tool3D.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 30/05/2014.
//
//

#pragma once
#include "tool.h"
#include "meshFaceSelector.h"
#include "meshVertexSelector.h"

class apparelModel;
class apparelMod;
class toolMods;
class tool3D : public tool
{
	public:
		tool3D				(toolManager* parent, apparelModel* model);

		void				createControlsCustom();
		void				update				();
		void				drawUI				();
		void				draw				();
		void				handleEvents		(ofxUIEventArgs& e);
		void				mousePressed		(int x, int y, int button);
		void				onSelectMod			(apparelMod*);
	
		toolMods*			mp_toolMods;
		apparelMod*			mp_apparelModCurrent;
	
		apparelModel*		mp_apparelModel;
		meshFaceSelector	m_meshFaceSelector;
		meshVertexSelector	m_meshVertexSelector;
	
		bool				m_bShowVertexNormals;
		bool				m_bShowFaceNormals;
		ofMeshFace*			mp_meshFaceOver;
		int					m_meshFaceIndexOver;
		ofVec3f*			mp_meshVertexOver;
		ofVec2f				m_meshVertexScreenOver;
	
		ofSpherePrimitive	m_sphereVertexOver;
	
		vector<int>			m_indicesFaceSelected;
		vector<int>			m_indicesVectorSelected;
	
		bool				isFaceSelected		(int index);
	
		ofColor				colorFaceOver;
		ofColor				colorFaceSelected;
	

		enum enumSelection
		{
			E_selection_vertex 	= 0,
			E_selection_edge 	= 1,
			E_selection_face 	= 2
		};
	
		enumSelection		m_selection;
};