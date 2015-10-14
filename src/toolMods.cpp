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
#include "data.h"

#define TOOLMODS_SIZE_NORMALS	4.0


//--------------------------------------------------------------
toolMods::toolMods(toolManager* parent, apparelModManager* modManager) : tool("Mods", parent)
{
	mp_modUICurrent = 0;
	mp_moodUICurrent= 0;
	mp_modsManager 	= modManager;

	m_bShowVertexNormals 	= false;
	m_bShowVertexIndices	= false;
	m_bShowFaceNormals		= false;
	m_bShowFaceIndices		= false;
	m_bEnableBackFaceCulling= false;
	m_bDrawWireFrameOnly	= false;
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
	
	m_bViewMixed			= false;
	m_bPostProcess			= false;
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
	if (mp_moodUICurrent)
		mp_moodUICurrent->show(is);
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

	mp_canvas->addToggle("show vertex normals", 	&m_bShowVertexNormals, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("show vertex indices", 	&m_bShowVertexIndices, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("show face normals", 		&m_bShowFaceNormals, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("show face indices", 		&m_bShowFaceIndices, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("enable backface culling",	&m_bEnableBackFaceCulling, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	mp_canvas->addToggle("draw wireframe only", 	&m_bDrawWireFrameOnly, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);

	mp_canvas->addWidgetDown(new ofxUILabel("View", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));
	mp_canvas->addToggle("mixed", 	&m_bViewMixed, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);

	mp_canvas->addWidgetDown(new ofxUILabel("Selection", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	vector<string> selectionIds;
	selectionIds.push_back("Vertex");
	selectionIds.push_back("Face");

	ofxUIRadio* radioSelect = mp_canvas->addRadio("Selection", selectionIds, OFX_UI_ORIENTATION_HORIZONTAL,OFX_UI_FONT_SMALL);

	mp_canvas->addWidgetDown(new ofxUILabelButton("Clear",false,300,OFX_UI_GLOBAL_BUTTON_DIMENSION,0,0,OFX_UI_FONT_SMALL));


	mp_canvas->addWidgetDown(new ofxUILabel("Camera", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	ofxUISlider* pSliderCamDist = new ofxUISlider("Distance", 40.0, 250.0, 100.0, 300, 20);
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
		if ((*it)->isMood() == false)
			modIds.push_back( (*it)->getId() );
	}
	
	mp_canvas->addLabel("Mods");
	mp_canvas->addSpacer(300,1);
	mp_canvas->addRadio("modIds", modIds);


	mp_canvas->addLabel("Moods");
	mp_canvas->addSpacer(300,1);
	vector<string> moodIds;

	map<string, apparelMod*>::iterator it2;

	for (it2 = mp_modsManager->m_moods.begin(); it2 != mp_modsManager->m_moods.end(); ++it2)
	{
			moodIds.push_back( it2->second->getId() );
	}
	mp_canvas->addRadio("moodIds", moodIds);


	mp_canvas->addWidgetDown(new ofxUILabel("PostProcess", OFX_UI_FONT_SMALL));
	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

	mp_canvas->addToggle("enable", 	&m_bPostProcess, 	OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);


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


	for (it = mp_modsManager->m_moods.begin(); it != mp_modsManager->m_moods.end(); ++it)
	{
		apparelModUI* modUI = makeInstanceModUI(it->second);
		if (modUI)
		{
			// Canavs position
			ofVec2f pos(
				getCanvas()->getRect()->getX() + 2*(getCanvas()->getRect()->getWidth()+10),
				getCanvas()->getRect()->getY()
			);
			
			// Create controls
			modUI->createControls( pos );
			
			// Save in map
			m_moodsUI[it->first] = modUI;
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
	

	vector<ofMeshFaceApparel*>& meshFaces = mp_apparelModel->getMeshFacesRef();
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
	apparelModel* pModel = m_bViewMixed ? mp_modsManager->getModelLastInChain() : mp_apparelModel;
	apparelMod*	  pMod = m_bViewMixed ? mp_modsManager->getModLastInChain() : mp_apparelModCurrent;

	if (pMod)
	{
		ofDrawAxis(100);

	   // MODEL
		if (m_bEnableBackFaceCulling)
		{
		   glEnable(GL_CULL_FACE);
		}
		
		if (m_bDrawWireFrameOnly == false)
		{
/* 		    ofSetColor(0,255);
			ofLight light;
			ofColor color;
		    color.setBrightness(150);
    		light.setDiffuseColor(color);
    		light.enable();
			pModel->getMeshRef().enableNormals();
		 // enableNormals();

			
			ofEnableLighting();
			glShadeModel(GL_FLAT);
*/
		   ofSetColor(0,255);
		   pMod->drawFaces();

//			ofDisableLighting();
		}

	   if (m_bDrawWireFrameOnly == false)
	   {
	   	glEnable(GL_POLYGON_OFFSET_LINE);
	   	glPolygonOffset(-1,-1);
	   }
	 
	   ofSetColor(255,255);
	   pMod->drawWireframe();

	   if (m_bDrawWireFrameOnly == false)
	   {
	   	glDisable(GL_POLYGON_OFFSET_LINE);
	   }
	 
	   if (m_bEnableBackFaceCulling)
	   {
		   glDisable(GL_CULL_FACE);
	   }

	   // Draw data from this tool
	   // only in non-mixed view
	   if (m_bViewMixed == false)
	   {
		   if (mp_apparelModCurrent)
		   {
				mp_apparelModCurrent->drawExtra();
		   }
		   
		   drawVertexNormals(pModel);
		   drawFaceNormals(pModel);
		   drawSelection(pModel);
		}
	}
	
	if (m_bViewMixed)
	{
		mp_modsManager->drawModsExtra();
	}
}

//--------------------------------------------------------------
void toolMods::drawVertexNormals(apparelModel* pModel)
{
	if (m_bShowVertexNormals)
	{
		vector<ofVec3f>& apparelModelNormals 		= pModel->getNormalsRef();
	 	vector<ofVec3f>& apparelModelVert 			= pModel->getVerticesRef();
	 	vector<ofIndexType>& apparelModelIndices 	= pModel->getIndicesRef();

	 	ofPushMatrix();
	 	ofVec3f v, n;
		string strVertexIndex;
		float strFontScale = 0.15;
	 	for (int i=0; i<apparelModelVert.size(); i++)
	 	{
			v = apparelModelVert[i];
		 	n = TOOLMODS_SIZE_NORMALS*apparelModelNormals[i];


		 	ofSetColor(0,255,0);
		 	ofLine(v.x,v.y,v.z, v.x+n.x,v.y+n.y,v.z+n.z);
	 	}
	 	ofPopMatrix();

		if (pModel && m_bShowVertexIndices && mp_meshFaceOver)
		{
			ofMeshFaceApparel* pFace = pModel->meshFaces[m_meshFaceIndexOver];

			for (int i=0;i<3;i++)
			{
				ofIndexType indexVertex = pFace->getVertexIndex(i);
			
				v = apparelModelVert[indexVertex];
			 	n = TOOLMODS_SIZE_NORMALS*apparelModelNormals[indexVertex];
				strVertexIndex = "["+ofToString(pFace->getOffsetIndices())+"] "+ofToString( indexVertex );


				ofSetColor(255);
				ofPushMatrix();
				ofTranslate(v+0.25*n);
				ofRotateX(90);
				ofScale(strFontScale,strFontScale,strFontScale);
				DATA->m_fontGothic.drawString(strVertexIndex,0,0);
				ofPopMatrix();
			}
		   }

	}
}

//--------------------------------------------------------------
void toolMods::drawFaceNormals(apparelModel* pModel)
{
	// FACE NORMALS
	if (m_bShowFaceNormals)
	{
		 vector<ofMeshFaceApparel*>& meshFacesRef = pModel->getMeshFacesRef();
	
		ofMeshFaceApparel* pFace;
	 	ofVec3f normal, middle;
	 	ofSetColor(255,0,0);
		
		string strFaceIndex = "";
		float strFontScale = 0.15;
	 	for (int i=0; i<meshFacesRef.size();i++)
	 	{
			pFace 		= meshFacesRef[i];
		 	normal 	= TOOLMODS_SIZE_NORMALS *pFace->getFaceNormal();
		 	middle 	= (pFace->getVertex(0) + pFace->getVertex(1) + pFace->getVertex(2))/3.0;

		 	ofPushMatrix();
		 	ofTranslate(middle.x,middle.y,middle.z);
		 	ofLine(0,0,0, normal.x,normal.y,normal.z);
			if (m_bShowFaceIndices)
			{
				strFaceIndex = ofToString(i);

		 		ofTranslate(normal.x,normal.y,normal.z);
				ofRotateX(90);
				ofRectangle strRect = DATA->m_fontGothic.getStringBoundingBox(strFaceIndex, 0, 0);
				ofTranslate(-strRect.getWidth()/2*strFontScale,-strRect.getHeight()/2*strFontScale,0);
				ofScale(strFontScale,strFontScale,strFontScale);
				DATA->m_fontGothic.drawString(strFaceIndex,0,0);
			}


		 	ofPopMatrix();
	 	}
	}
	
}

//--------------------------------------------------------------
void toolMods::drawSelection(apparelModel* pModel)
{

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
		if (mp_meshFaceOver && m_meshFaceIndexOver>=0)
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
		if (pModel)
		{
			vector<int>::iterator it;
			for (it = mp_apparelModCurrent->m_indicesFaces.begin(); it != mp_apparelModCurrent->m_indicesFaces.end(); ++it)
			{
				int faceIndex = *it;
				ofMeshFaceApparel* pFace = mp_apparelModel->getMeshFacesRef()[faceIndex];

				ofVec3f faceNormal = 0.5*pFace->getFaceNormal();
				ofSetColor(colorFaceSelected,60);
				ofFill();
				ofTriangle(pFace->getVertex(0)+faceNormal,pFace->getVertex(1)+faceNormal,pFace->getVertex(2)+faceNormal);
			}
		}
	}
}

//--------------------------------------------------------------
void toolMods::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();

	if (isNameMod(name))
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if (toggle && toggle->getValue()) selectMod(name);
	}
	if (isNameMood(name))
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if (toggle && toggle->getValue()) selectMood(name);
	}
	else if (name == "Selection")
	{
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		m_selection = (enumSelection)radio->getValue();
		ofLog() << "tool3D::handleEvents() - selection=" << ofToString((int)m_selection);
	}
	else if (name == "Clear")
	{
		if (e.getButton()->getValue()>0)
			unselectSelection();

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
	if (mp_meshFaceOver && m_meshFaceIndexOver>=0)
	{
		if (!isFaceSelected(m_meshFaceIndexOver))
		{
			mp_apparelModCurrent->addFaceIndex(m_meshFaceIndexOver);
			mp_meshFaceOver = 0;
			m_meshFaceIndexOver = -1;
			
			isModelModified = true;
		}
		else{
			mp_apparelModCurrent->removeFaceIndex(m_meshFaceIndexOver);
			isModelModified = true;
		}
	}
	
	// VERTEX ?
	if (mp_meshVertexOver && m_meshVertexIndexOver>=0)
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
		mp_apparelModCurrent->setChanged();
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
void toolMods::selectMood(string name)
{
	// Show UI
	map<string, apparelModUI*>::iterator it;
	for (it = m_moodsUI.begin(); it != m_moodsUI.end(); ++it){
		it->second->hide();
	}

	// Select mood
	mp_modsManager->selectMood(name);

	// Select mood UI
	mp_moodUICurrent = m_moodsUI[name];
	if (mp_moodUICurrent)
		mp_moodUICurrent->show();

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

//--------------------------------------------------------------
bool toolMods::isNameMood(string id)
{
	if (mp_toolManager && mp_modsManager->getMood(id))
		return true;
	return false;
}


//--------------------------------------------------------------
void toolMods::unselectSelection()
{
	if (mp_apparelModCurrent)
	{
		if (m_selection == E_selection_vertex)
		{
			mp_apparelModCurrent->clearSelectionVertices();
			m_mapSphereVertexSelected.clear();
		}
		else if (m_selection == E_selection_face)
			mp_apparelModCurrent->clearSelectionFaces();
	}
}



