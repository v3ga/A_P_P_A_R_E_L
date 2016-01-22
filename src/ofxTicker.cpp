//
//  ofxTicker.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 22/07/2014.
//
//

#include "ofxTicker.h"

//--------------------------------------------------------------
ofxTicker::ofxTicker()
{
	reset();
	ofAddListener(ofEvents().update, this, &ofxTicker::update);
}

//--------------------------------------------------------------
void ofxTicker::reset()
{
	m_time 			= 0.0f;
	m_timePeriod	= 0.0f;
	m_period		= 1.0f;
	m_isPlaying		= false;
}

//--------------------------------------------------------------
void ofxTicker::play()
{
	m_isPlaying = true;
	m_timePeriod = m_period;
}

//--------------------------------------------------------------
void ofxTicker::update(ofEventArgs& args)
{
	if (m_isPlaying == false) return;

	float dt = (float) ofGetLastFrameTime();

	m_time += dt;
	m_timePeriod += dt;

	if (m_timePeriod >= m_period)
	{
		newTickEventArgs.time 	= m_time;
		newTickEventArgs.dt 	= dt;
		ofNotifyEvent(newTickEvent, newTickEventArgs, this);

		m_timePeriod = 0.0f;
	}
}
