//
//  apparelModUI.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 02/06/2014.
//
//

#include "apparelModUI.h"
#include "parameterGroupLowHigh.h"
#include "globals.h"

//--------------------------------------------------------------
apparelModUI::apparelModUI(apparelMod* mod)
{
	mp_mod = mod;
	mp_canvas = 0;
	mp_sliderWeight = 0;
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

	if (mp_mod->isMood() == false)
		mp_canvas->addWidgetDown( new ofxUILabelButton("reset sql", false, 100,16,0,0,OFX_UI_FONT_SMALL) );
	mp_canvas->addWidgetDown( new ofxUILabel("params","Parameters",OFX_UI_FONT_SMALL) );
    mp_canvas->addSpacer(300,1);

	
	ofParameterGroup& parametersGroup = mp_mod->getParamatersGroupRef();
	int nbParameters = parametersGroup.size();
	for (int i=0;i<nbParameters;i++)
	{
		ofAbstractParameter& parameter = parametersGroup[i];
		if (parameter.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& parameterFloat = (ofParameter<float>&) parameter;
			mp_canvas->addSlider(parameterFloat.getName(), parameterFloat.getMin(), parameterFloat.getMax(), parameterFloat);
		}
		else if (parameter.type() == typeid(ofParameter<bool>).name())
		{
			ofParameter<bool>& parameterBool = (ofParameter<bool>&) parameter;
			mp_canvas->addToggle(parameterBool.getName(), parameterBool);
		}
		if (parameter.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int>& parameterInt = (ofParameter<int>&) parameter;
			mp_canvas->addIntSlider(parameterInt.getName(), parameterInt.getMin(), parameterInt.getMax(), parameterInt);
		}
		else
		if (parameter.type() == typeid(ofParameterGroup).name())
		{
			ofParameterGroup& parameterGroup = (ofParameterGroup&) parameter;

			ofParameter<float>& parameterFloatMin 		= (ofParameter<float>&) parameterGroup.get("min");
			ofParameter<float>& parameterFloatMax 		= (ofParameter<float>&) parameterGroup.get("max");
			ofParameter<float>& parameterFloatLow 		= (ofParameter<float>&) parameterGroup.get("low");
			ofParameter<float>& parameterFloatHigh 		= (ofParameter<float>&) parameterGroup.get("high");

			mp_canvas->addRangeSlider(parameterGroup.getName(), parameterFloatMin.get(),parameterFloatMax.get(), parameterFloatLow.get(), parameterFloatHigh.get());
		}
	}


	mp_sliderWeight = mp_canvas->addSlider( "Weight", 0.0f, 1.0f, mp_mod->m_weight );

	mp_canvas->autoSizeToFitWidgets();
	
   ofAddListener(mp_mod->getParamatersGroupRef().parameterChangedE, this, &apparelModUI::parameterChanged);
   ofAddListener(mp_canvas->newGUIEvent, this, &apparelModUI::handleEvents);
}

//--------------------------------------------------------------
void apparelModUI::show(bool is)
{
	if (mp_canvas)
		mp_canvas->setVisible(is);
}

//--------------------------------------------------------------
void apparelModUI::parameterChanged(ofAbstractParameter& parameter)
{
	if (parameter.getName() == "WeightManual")
	{
		if (mp_sliderWeight)
			mp_sliderWeight->setValue(mp_mod->m_weight);
	}
}

//--------------------------------------------------------------
void apparelModUI::handleEvents(ofxUIEventArgs& e)
{
	if (mp_mod == 0) return;

    string name = e.widget->getName();
	
	if (name == "reset sql")
	{
		ofxUILabelButton* pBtn = (ofxUILabelButton*) e.widget;
		if (pBtn->getValue())
		{
			if (mp_mod)
				mp_mod->resetWordsCountUserDatabase( GLOBALS->getUser() );
		}
	}
	else
	if (name == "Weight")
	{
		if (mp_mod)
		{
			if (mp_mod->m_isWeightManual)
			{
				mp_mod->setWeight( e.getSlider()->getScaledValue() );
			}
		}
	}
	else
	{
	   ofParameterGroup& parametersGroup = mp_mod->getParamatersGroupRef();
	   ofAbstractParameter& parameter = parametersGroup.get(name);


	   if (parameter.type() == typeid(ofParameter<float>).name())
	   {
		   ofParameter<float>& parameterFloat = (ofParameter<float>&) parameter;
		   parameterFloat.set( ((ofxUISlider *) e.widget)->getScaledValue() );
	   }
	   else
	   if (parameter.type() == typeid(ofParameter<bool>).name())
	   {
		   ofParameter<bool>& parameterBool = (ofParameter<bool>&) parameter;
		   parameterBool.set( ((ofxUIToggle *) e.widget)->getValue() );
	   }
	   else
	   if (parameter.type() == typeid(ofParameter<int>).name())
	   {
		   ofParameter<int>& parameterInt = (ofParameter<int>&) parameter;
		   parameterInt.set( ((ofxUIIntSlider *) e.widget)->getValue() );
	   }
	   else
	   if (parameter.type() == typeid(ofParameterGroup).name())
	   {
		   ofParameterGroup& parameterGroup = (ofParameterGroup&) parameter;
				
		   ofParameter<float>& parameterFloatLow 		= (ofParameter<float>&) parameterGroup.get("low");
		   ofParameter<float>& parameterFloatHigh 		= (ofParameter<float>&) parameterGroup.get("high");

		   ofxUIRangeSlider* pRangeSlider = (ofxUIRangeSlider*) e.widget;
		   parameterFloatLow.set( pRangeSlider->getValueLow() );
		   parameterFloatHigh.set( pRangeSlider->getValueHigh() );
	   }
	
	}
	
}





