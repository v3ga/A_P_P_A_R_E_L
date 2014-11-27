//
//  toolMods.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#include "toolMods.h"
#include "tool3D.h"
#include "globals.h"
#include "apparelModManager.h"
#include "oscSender.h"
#include "ofApp.h"

#define TOOLMODS_SIZE_NORMALS	4.0


//--------------------------------------------------------------
toolMods::toolMods(toolManager* parent, apparelModManager* modManager) : tool("Mods", parent)
{
	mp_modUICurrent = 0;
	mp_modsManager 	= modManager;

	m_bShowVertexNormals 	= false;
	m_bShowFaceNormals		= false;
	mp_meshFaceOver			= 0;
	m_meshFaceIndexOver		= -1;
	m_meshVertexIndexOver	= -1;
	mp_meshVertexOver		= 0;
	mp_apparelModel			= 0;
	
	mp_lblModel				= 0;
	
	m_sphereVertexOver.setRadius(3.0f);
	m_sphereVertexOver.setResolution(2);
	
	colorFaceOver.set(255,0,0);
	colorFaceSelected.set(255,255,255);
}

//--------------------------------------------------------------
toolMods::~toolMods()
{
}

//--------------------------------------------------------------
void toolMods::show(bool is)
{
	tool::show(is);
	if (mp_modUICurrent)
		mp_modUICurrent->show(is);
}

//--------------------------------------------------------------
bool toolMods::isHit(int x, int y)
{
	return (tool::isHit(x, y) || ( mp_modUICurrent && mp_modUICurrent->getCanvas() &&  mp_modUICurrent->getCanvas()->isHit(x,y)));
}

//--------------------------------------------------------------
void toolMods::exit()
{
	if (mp_modsManager)
	{
		mp_modsManager->saveModel();
		mp_modsManager->saveParameters();
		mp_modsManager->deleteMods();
	}
}


//--------------------------------------------------------------
void toolMods::createControlsCustom()
{
	if (mp_modsManager == 0) return;

	// 3D Selection UI
	mp_lblModel = mp_canvas->addLabel("Model :" + (mp_apparelModel ? mp_apparelModel->getId() : "???"));
	mp_canvas->addSpacer(300,1);

	mp_canvas->addToggle("show vertex normals", m_bShowVertexNormals, OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("show face normals", m_bShowFaceNormals, OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);

	mp_canvas->addWidgetDown(new ofxUILabel("Selection", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	vector<string> selectionIds;
	selectionIds.push_back("Vertex");
	selectionIds.push_back("Face");

	ofxUIRadio* radioSelect = mp_canvas->addRadio("Selection", selectionIds, OFX_UI_ORIENTATION_HORIZONTAL,OFX_UI_FONT_SMALL);


	mp_canvas->addWidgetDown(new ofxUILabel("Camera", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	ofxUISlider* pSliderCamDist = new ofxUISlider("Distance", 40.0, 150.0, 100.0, 300, 20);
	mp_canvas->addWidgetDown(pSliderCamDist);

/*
	mp_canvas->addWidgetDown(new ofxUILabel("Scene", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	mp_canvas->setVisible(false);
	mp_canvas->autoSizeToFitWidgets();
 */

	// Mods UI
	vector<string> modIds;

	vector<apparelMod*>::iterator it;
	for (it = mp_modsManager->m_modsChain.begin(); it != mp_modsManager->m_modsChain.end(); ++it)
	{
		modIds.push_back( (*it)->getId() );
	}
	
	mp_canvas->addLabel("Mods");
	mp_canvas->addSpacer(300,1);
	mp_canvas->addRadio("modIds", modIds);
	mp_canvas->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void toolMods::createControlsCustomFinalize()
{
	if (mp_modsManager == 0) return;

	map<string, apparelMod*>::iterator it;
	for (it = mp_modsManager->m_mods.begin(); it != mp_modsManager->m_mods.end(); ++it)
	{
		apparelModUI* modUI = makeInstanceModUI(it->second);
		if (modUI)
		{
			// Canavs position
			ofVec2f pos(
				getCanvas()->getRect()->getX() + getCanvas()->getRect()->getWidth()+10,
				getCanvas()->getRect()->getY()
			);
			
			// Create controls
			modUI->createControls( pos );
			
			// Save in map
			m_modsUI[it->first] = modUI;
			
		}
	}
}

//--------------------------------------------------------------
void toolMods::update()
{
	if (mp_modsManager)
	{
		mp_modsManager->applyModChain();
	}


	if (mp_apparelModel == 0) return;

	vector<ofMeshFace>& meshFaces = mp_apparelModel->getMeshFacesRef();
	ofCamera& cam = GLOBALS->getApp()->cam;

	if (m_selection == E_selection_vertex)
	{
		meshFaceSelectorResult* pResult = m_meshVertexSelector.select(mp_apparelModel->getMeshRef(), cam, ofVec2f(GLOBALS->getApp()->mouseX,GLOBALS->getApp()->mouseY));
		mp_meshVertexOver = pResult->vertex;
		m_meshVertexIndexOver = pResult->vertexIndex;
		m_meshVertexScreenOver = pResult->vertexScreen;
	}
	else if (m_selection == E_selection_face)
	{
		meshFaceSelectorResult* pResult = m_meshFaceSelector.select(meshFaces, cam, ofVec2f(GLOBALS->getApp()->mouseX,GLOBALS->getApp()->mouseY));
		mp_meshFaceOver = pResult->face;
		m_meshFaceIndexOver = pResult->faceIndex;
	}

}

//--------------------------------------------------------------
void toolMods::draw()
{

	if (mp_apparelModel == 0) return;

	 vector<ofVec3f>& apparelModelVert 		= mp_apparelModel->getVerticesRef();
	 vector<ofVec3f>& apparelModelNormals 	= mp_apparelModel->getNormalsRef();


	// MODEL
	ofSetColor(0);
	mp_apparelModel->drawFaces();

	ofSetColor(255);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1,-1);
	mp_apparelModel->drawWireframe();
	glDisable(GL_POLYGON_OFFSET_LINE);


	// VERTEX NORMALS
	if (m_bShowVertexNormals)
	{

	 ofPushMatrix();
	 ofVec3f v, n;
	 ofSetColor(0,255,0);
	 for (int i=0; i<apparelModelVert.size(); i++)
	 {
		 v = apparelModelVert[i];
		 n = TOOLMODS_SIZE_NORMALS*apparelModelNormals[i];
		 ofLine(v.x,v.y,v.z, v.x+n.x,v.y+n.y,v.z+n.z);
	 }
	 ofPopMatrix();
	}
	
	// FACE NORMALS
	if (m_bShowFaceNormals)
	{
	 vector<ofMeshFace>& meshFacesRef = mp_apparelModel->getMeshFacesRef();

	 ofMeshFace face;
	 ofVec3f normal, middle;
	 ofSetColor(255,0,0);
	 for (int i=0; i<meshFacesRef.size();i++)
	 {
		 face 		= meshFacesRef[i];
		 normal 	= TOOLMODS_SIZE_NORMALS *meshFacesRef[i].getFaceNormal();
		 middle 	= (face.getVertex(0) + face.getVertex(1) + face.getVertex(2))/3;

		 ofPushMatrix();
		 ofTranslate(middle.x,middle.y,middle.z);
		 ofLine(0,0,0, normal.x,normal.y,normal.z);
		 ofTranslate(normal.x,normal.y,normal.z);
		 //ofDrawBitmapString(ofToString(i), normal);
 		//font.drawString(ofToString(i), 0, 0);

		 ofPopMatrix();
	 }
	}


	if (m_selection == E_selection_vertex)
	{
		if (mp_meshVertexOver && !isVertexSelected(m_meshVertexIndexOver))
		{
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,0,0,160);
			ofPushMatrix();
			ofTranslate(*mp_meshVertexOver);
			m_sphereVertexOver.drawWireframe();
			ofPopMatrix();
			ofPopStyle();
		}

		if (mp_apparelModCurrent)
		{
			vector<int>::iterator it;
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,0,0,160);
			for (it = mp_apparelModCurrent->m_indicesVertex.begin(); it != mp_apparelModCurrent->m_indicesVertex.end(); ++it)
			{
				ofSpherePrimitive* pShere = m_mapSphereVertexSelected[*it];
				if (pShere)
				{
					ofVec3f position = mp_apparelModel->getVerticesRef()[*it];

					ofPushMatrix();
					ofTranslate(position);
					pShere->drawWireframe();
					ofPopMatrix();
				}
			
			}
			ofPopStyle();
		}


	}
	else if (m_selection == E_selection_face)
	{
		if (mp_meshFaceOver && m_meshFaceIndexOver>0)
		{
			if (!isFaceSelected(m_meshFaceIndexOver))
			{
				ofVec3f faceNormal = 0.5*mp_meshFaceOver->getFaceNormal();
				ofSetColor(colorFaceOver,60);
				ofFill();
//				ofTriangle(mp_meshFaceOver->getVertex(0)+faceNormal,mp_meshFaceOver->getVertex(1)+faceNormal,mp_meshFaceOver->getVertex(2)+faceNormal);
				glEnable(GL_POLYGON_OFFSET_FILL);
			    glPolygonOffset(-1,-1);
				ofTriangle(mp_meshFaceOver->getVertex(0),mp_meshFaceOver->getVertex(1),mp_meshFaceOver->getVertex(2));
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
		}
		if (mp_apparelModCurrent)
		{
			vector<int>::iterator it;
			for (it = mp_apparelModCurrent->m_indicesFaces.begin(); it != mp_apparelModCurrent->m_indicesFaces.end(); ++it)
			{
				ofMeshFace face = mp_apparelModel->getMeshFacesRef()[*it];

				ofVec3f faceNormal = 0.5*face.getFaceNormal();
				ofSetColor(colorFaceSelected,60);
				ofFill();
				ofTriangle(face.getVertex(0)+faceNormal,face.getVertex(1)+faceNormal,face.getVertex(2)+faceNormal);
			}
		}
	}

}


//--------------------------------------------------------------
void toolMods::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();

//	ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//	if (toggle && toggle->getValue()) setTool(name);

	// NEVER CALLED
/*
	if (name == "modIds")
	{
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		selectMod( radio->getActiveName() );
	}
*/
	if (isNameMod(name))
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if (toggle && toggle->getValue()) selectMod(name);
	}
	else if (name == "show vertex normals")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		m_bShowVertexNormals = toggle->getValue();
		
	}
	else if (name == "show face normals")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		m_bShowFaceNormals = toggle->getValue();
		
	}
	else if (name == "Selection")
	{
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		m_selection = (enumSelection)radio->getValue();
		ofLog() << "tool3D::handleEvents() - selection=" << ofToString((int)m_selection);
	}
	else if (name == "Distance")
	{
        ofxUISlider* slider = (ofxUISlider *) e.widget;
		GLOBALS->getApp()->cam.setDistance( slider->getScaledValue() );
	}
}

//--------------------------------------------------------------
void toolMods::mousePressed(int x, int y, int button)
{
	if (mp_apparelModCurrent==0) return;

	bool isModelModified = false;

	// FACE ?
	if (mp_meshFaceOver && m_meshFaceIndexOver>0)
	{
		if (!isFaceSelected(m_meshFaceIndexOver))
		{
			mp_apparelModCurrent->addFaceIndex(m_meshFaceIndexOver);
			//m_indicesFaceSelected.push_back(m_meshFaceIndexOver);
			mp_meshFaceOver = 0;
			m_meshFaceIndexOver = -1;
			
			isModelModified = true;
		}
		else{
			mp_apparelModCurrent->removeFaceIndex(m_meshFaceIndexOver);
			//m_indicesFaceSelected.erase(std::remove(m_indicesFaceSelected.begin(), m_indicesFaceSelected.end(), m_meshFaceIndexOver), m_indicesFaceSelected.end());
			isModelModified = true;
		}
	}
	
	// VERTEX ?
	if (mp_meshVertexOver && m_meshVertexIndexOver>0)
	{
		if (!isVertexSelected(m_meshVertexIndexOver))
		{
			mp_apparelModCurrent->addVertexIndex(m_meshVertexIndexOver);
			ofSpherePrimitive* pSphere = new ofSpherePrimitive();
			pSphere->setRadius(3);
			pSphere->setResolution(4);
	//		pSphere->setMode(OF_PRIMITIVE_LINES);
			m_mapSphereVertexSelected[m_meshVertexIndexOver] = pSphere;

			mp_meshVertexOver = 0;
			m_meshVertexIndexOver = -1;
			isModelModified = true;
		}
		else{
			mp_apparelModCurrent->removeVertexIndex(m_meshVertexIndexOver);
			m_mapSphereVertexSelected.erase(m_meshVertexIndexOver);
			isModelModified = true;
		}
	}
	
	if (isModelModified)
	{
		OSC_SENDER->sendModData(mp_apparelModCurrent);
	}
}

//--------------------------------------------------------------
apparelModUI* toolMods::makeInstanceModUI(apparelMod* mod)
{
	return new apparelModUI(mod);
}


//--------------------------------------------------------------
void toolMods::selectMod(string name)
{
	OFAPPLOG->begin("toolMods::onSelectMod(\""+name+"\")");

	// Show UI
	map<string, apparelModUI*>::iterator it;
	for (it = m_modsUI.begin(); it != m_modsUI.end(); ++it){
		it->second->hide();
	}

	// Select mod
	mp_modsManager->selectMod(name);

	// Select mod UI
	mp_modUICurrent = m_modsUI[name];
	if (mp_modUICurrent)
		mp_modUICurrent->show();
	
	// Inform other tool
	mp_apparelModCurrent = mp_modsManager->getModCurrent();

	if (mp_apparelModCurrent)
	{
		mp_apparelModel = &mp_apparelModCurrent->m_model;

		m_mapSphereVertexSelected.clear();
		vector<int>::iterator it;
		for (it = mp_apparelModCurrent->m_indicesVertex.begin(); it != mp_apparelModCurrent->m_indicesVertex.end(); ++it){

			ofSpherePrimitive* pSphere = new ofSpherePrimitive();
			pSphere->setRadius(3);
			pSphere->setResolution(4);
			m_mapSphereVertexSelected[*it] = pSphere;
		
		}
	}
	
	// Inform on network
	OSC_SENDER->selectMod( mp_modsManager->getModCurrent() );

	OFAPPLOG->end();
}

//--------------------------------------------------------------
bool toolMods::isFaceSelected(int index)
{
	if (mp_apparelModCurrent==0) return false;

	vector<int>::iterator it;
	for (it = mp_apparelModCurrent->m_indicesFaces.begin(); it != mp_apparelModCurrent->m_indicesFaces.end(); ++it){
		if (*it == index) return true;
	}
	return false;
}

//--------------------------------------------------------------
bool toolMods::isVertexSelected(int index)
{
	if (mp_apparelModCurrent==0) return false;

	vector<int>::iterator it;
	for (it = mp_apparelModCurrent->m_indicesVertex.begin(); it != mp_apparelModCurrent->m_indicesVertex.end(); ++it){
		if (*it == index) return true;
	}
	return false;
}

//--------------------------------------------------------------
bool toolMods::isNameMod(string id)
{
	if (mp_toolManager && mp_modsManager->getMod(id))
		return true;
	return false;
}


