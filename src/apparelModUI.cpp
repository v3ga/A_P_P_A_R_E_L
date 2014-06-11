//
//  apparelModUI.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#include "apparelModUI.h"

//--------------------------------------------------------------
apparelModUI::apparelModUI(apparelMod* mod)
{
	mp_mod = mod;
	mp_canvas = 0;
}

//--------------------------------------------------------------
apparelModUI::~apparelModUI()
{
	delete mp_canvas;
}

//--------------------------------------------------------------
void apparelModUI::createControls(const ofVec2f& posCanvas)
{
   	mp_canvas = new ofxUICanvas(posCanvas.x,posCanvas.y, 200,300);
	mp_canvas->setFont("fonts/LetterGothic.ttf");
	mp_canvas->setVisible(false);

	mp_canvas->addLabel(mp_mod->m_id);
	mp_canvas->addSpacer(300,1);
	
	ofParameterGroup& parametersGroup = mp_mod->getParamatersGroupRef();
	int nbParamaters = parametersGroup.size();
	for (int i=0;i<nbParamaters;i++)
	{
		ofAbstractParameter& parameter = parametersGroup[i];
		if (parameter.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& parameterFloat = (ofParameter<float>&) parameter;
			mp_canvas->addSlider(parameterFloat.getName(), parameterFloat.getMin(), parameterFloat.getMax(), parameterFloat);
		}
	}
	mp_canvas->autoSizeToFitWidgets();
	
   ofAddListener(mp_canvas->newGUIEvent, this, &apparelModUI::handleEvents);
}

//--------------------------------------------------------------
void apparelModUI::show(bool is)
{
	if (mp_canvas)
		mp_canvas->setVisible(is);
}


//--------------------------------------------------------------
void apparelModUI::handleEvents(ofxUIEventArgs& e)
{

}





