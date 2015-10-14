//
//  apparelMod_selfopathy.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 03/02/2015.
//
//

#pragma once

#include "apparelMod.h"
#include "ofxButterfly.h"
#include "BoundingBox.h"

class apparelMod_selfopathy : public apparelMod
{
	public:
		apparelMod_selfopathy	();
		~apparelMod_selfopathy	();


/*		void					drawFaces			();
*/
		void				apply				();
		void				update				();
		void				drawExtra			();
		void				onParameterChanged	(ofAbstractParameter& parameter);
		void				onWeightChanged		();
		


	private:
		ofImage						m_image;

	    ofxButterfly 				butterfly;
		ofMesh 						newMesh;

 
		ofParameter<float>			m_amplitude;
		ofParameter<float>			m_levelSubdiv;
 		ofParameterGroup			m_amplitudeRndFactor;

		BoundingBox					m_meshInputBoundingBox;
 
		void						displaceVertices();
};