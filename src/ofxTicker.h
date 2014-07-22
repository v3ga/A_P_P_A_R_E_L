//
//  ofxTicker.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 22/07/2014.
//
//

#pragma once
#include "ofMain.h"

class ofxTickerEventArgs : public ofEventArgs
{
	public:

		float		time;
		float		dt;
};


class ofxTicker
{
	public:
		ofxTicker();
	
		virtual	void					reset		();
		virtual	void					play		();
		virtual void					update		(ofEventArgs& args);
				void					setPeriod	(float period){m_period = period;}

		ofxTickerEventArgs				newTickEventArgs;
		ofEvent<ofxTickerEventArgs> 	newTickEvent;


	protected:
		float							m_time;
		float							m_timePeriod;
		float							m_period;	// seconds
		bool							m_isPlaying;

};
