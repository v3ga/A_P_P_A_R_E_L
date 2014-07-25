//
//  tool3D.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 30/05/2014.
//
//

#include "tool3D.h"
#include "toolMods.h"
#include "globals.h"
#include "ofApp.h"

//--------------------------------------------------------------
tool3D::tool3D(toolManager* parent, apparelModel* model) : tool("3D", parent)
{
	mp_toolMods				= 0;
	mp_apparelModCurrent	= 0;

	m_bShowVertexNormals 	= false;
	m_bShowFaceNormals		= false;
	mp_meshFaceOver			= 0;
	m_meshFaceIndexOver		= -1;
	m_meshVertexIndexOver	= -1;
	mp_meshVertexOver		= 0;
	mp_apparelModel			= model;
	
	m_sphereVertexOver.setRadius(3.0f);
	m_sphereVertexOver.setResolution(2);
	
	colorFaceOver.set(255,0,0);
	colorFaceSelected.set(255,255,255);
}

//--------------------------------------------------------------
void tool3D::createControlsCustom()
{
	if (mp_canvas)
	{
	
		mp_canvas->addWidgetDown(new ofxUILabel("Debug", OFX_UI_FONT_SMALL));
    	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));
	    mp_canvas->addToggle("show vertex normals", m_bShowVertexNormals, OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);
	    mp_canvas->addToggle("show face normals", m_bShowFaceNormals, OFX_UI_GLOBAL_BUTTON_DIMENSION, OFX_UI_GLOBAL_BUTTON_DIMENSION);

		mp_canvas->addWidgetDown(new ofxUILabel("Selection", OFX_UI_FONT_SMALL));
    	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

		vector<string> selectionIds;
		selectionIds.push_back("Vertex");
// TODO : later ? use point selector
//		selectionIds.push_back("Edge");
		selectionIds.push_back("Face");

		ofxUIRadio* radioSelect = mp_canvas->addRadio(_id("Selection"), selectionIds, OFX_UI_ORIENTATION_HORIZONTAL,OFX_UI_FONT_SMALL);


		mp_canvas->addWidgetDown(new ofxUILabel("Camera", OFX_UI_FONT_SMALL));
    	mp_canvas->addWidgetDown(new ofxUISpacer(300, 1));

		ofxUISlider* pSliderCamDist = new ofxUISlider(_id("Distance"), 100.0, 300.0, 200.0, 300, 20);
		mp_canvas->addWidgetDown(pSliderCamDist);

		mp_canvas->setVisible(false);
		mp_canvas->autoSizeToFitWidgets();
		
	}
	
	mp_toolMods = (toolMods*)mp_toolManager->getTool("Mods");
}

//--------------------------------------------------------------
void tool3D::update()
{
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
void tool3D::draw()
{
	if (mp_apparelModel == 0) return;

	 vector<ofVec3f>& apparelModelVert 		= mp_apparelModel->getVerticesRef();
	 vector<ofVec3f>& apparelModelNormals 	= mp_apparelModel->getNormalsRef();

	// VERTEX NORMALS
	if (m_bShowVertexNormals)
	{

	 ofPushMatrix();
	 ofVec3f v, n;
	 ofSetColor(0,255,0);
	 for (int i=0; i<apparelModelVert.size(); i++)
	 {
		 v = apparelModelVert[i];
		 n = 10.0*apparelModelNormals[i];
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
		 normal 	= 20*meshFacesRef[i].getFaceNormal();
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
				ofTriangle(mp_meshFaceOver->getVertex(0)+faceNormal,mp_meshFaceOver->getVertex(1)+faceNormal,mp_meshFaceOver->getVertex(2)+faceNormal);
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
void tool3D::drawUI()
{
/*	if (m_selection == E_selection_vertex)
	{
		if (mp_meshVertexOver)
		{
			ofSetColor(255,0,0);
			ofPushMatrix();
			ofTranslate(m_meshVertexScreenOver);
			ofCircle(0,0,5,5);
			ofPopMatrix();
		}

	}
*/
}


//--------------------------------------------------------------
void tool3D::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
	if (name == "show vertex normals")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		m_bShowVertexNormals = toggle->getValue();
		
	}
	else if (name == "show face normals")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		m_bShowFaceNormals = toggle->getValue();
		
	}
	else if (name == _id("Selection"))
	{
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		m_selection = (enumSelection)radio->getValue();
		ofLog() << "tool3D::handleEvents() - selection=" << ofToString((int)m_selection);
	}
	else if (name == _id("Distance"))
	{
        ofxUISlider* slider = (ofxUISlider *) e.widget;
		GLOBALS->getApp()->cam.setDistance( slider->getScaledValue() );
	}
}

//--------------------------------------------------------------
void tool3D::mousePressed(int x, int y, int button)
{
	if (mp_apparelModCurrent==0) return;

	// FACE ?
	if (mp_meshFaceOver && m_meshFaceIndexOver>0)
	{
		if (!isFaceSelected(m_meshFaceIndexOver))
		{
			mp_apparelModCurrent->addFaceIndex(m_meshFaceIndexOver);
			//m_indicesFaceSelected.push_back(m_meshFaceIndexOver);
			mp_meshFaceOver = 0;
			m_meshFaceIndexOver = -1;
		}
		else{
			mp_apparelModCurrent->removeFaceIndex(m_meshFaceIndexOver);
			//m_indicesFaceSelected.erase(std::remove(m_indicesFaceSelected.begin(), m_indicesFaceSelected.end(), m_meshFaceIndexOver), m_indicesFaceSelected.end());
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
		}
		else{
			mp_apparelModCurrent->removeVertexIndex(m_meshVertexIndexOver);
			m_mapSphereVertexSelected.erase(m_meshVertexIndexOver);
		}
	}
}

//--------------------------------------------------------------
bool tool3D::isFaceSelected(int index)
{
	if (mp_apparelModCurrent==0) return false;

	vector<int>::iterator it;
	for (it = mp_apparelModCurrent->m_indicesFaces.begin(); it != mp_apparelModCurrent->m_indicesFaces.end(); ++it){
		if (*it == index) return true;
	}
	return false;
}

//--------------------------------------------------------------
bool tool3D::isVertexSelected(int index)
{
	if (mp_apparelModCurrent==0) return false;

	vector<int>::iterator it;
	for (it = mp_apparelModCurrent->m_indicesVertex.begin(); it != mp_apparelModCurrent->m_indicesVertex.end(); ++it){
		if (*it == index) return true;
	}
	return false;

}

//--------------------------------------------------------------
void tool3D::onSelectMod(apparelMod* pMod)
{
	mp_apparelModCurrent = pMod;

	if (mp_apparelModCurrent)
	{
		m_mapSphereVertexSelected.clear();
		vector<int>::iterator it;
		for (it = mp_apparelModCurrent->m_indicesVertex.begin(); it != mp_apparelModCurrent->m_indicesVertex.end(); ++it){

			ofSpherePrimitive* pSphere = new ofSpherePrimitive();
			pSphere->setRadius(3);
			pSphere->setResolution(4);
			m_mapSphereVertexSelected[*it] = pSphere;
		
		}
	}
}






