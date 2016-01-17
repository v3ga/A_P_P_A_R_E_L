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

class apparelMod_selfopathy : public apparelMod, public ofThread
{
	public:
		apparelMod_selfopathy	();
		~apparelMod_selfopathy	();


/*		void					drawFaces			();
*/
		void				createParameters	();
		void				apply				();
		void				update				();
		void				drawExtra			();
		void				onParameterChanged	(ofAbstractParameter& parameter);
		void				onWeightChanged		();
		bool				isBusy				(){return m_isBusy;}

 
		void				setImage			(ofImage* pImage);
		void 				threadedFunction	();

	private:
		ofImage*					mp_image;
		bool						m_bDoDisplacement;
		bool						m_bDoSudivision;

	    ofxButterfly 				m_butterfly;
		ofMesh 						m_newMesh;
		ofMesh						m_meshRefDisplacement;

 
		ofParameter<float>			m_amplitude;
		ofParameter<int>			m_levelSubdiv;
 		ofParameterGroup			m_amplitudeRndFactor;

		BoundingBox					m_meshInputBoundingBox;
 
 		void						subdivide			();
		void						displaceVertices	();
 
		ofThread					m_threadSubdivision;
		bool						m_isBusy;
		bool						m_bRefresh;
 
		apparelModel				m_modelThread;
};