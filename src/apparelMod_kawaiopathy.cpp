//
//  apparelMod_kawaiopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 25/05/2015.
//
//

#include "apparelMod_kawaiopathy.h"
#include "ofAppLog.h"

apparelMod_kawaiopathy::apparelMod_kawaiopathy() : apparelMod("Kawaiopathy")
{
	OFAPPLOG->begin("apparelMod_kawaiopathy::apparelMod_selfopathy()");
	OFAPPLOG->end();
}

apparelMod_kawaiopathy::~apparelMod_kawaiopathy()
{

}

void apparelMod_kawaiopathy::readModel()
{
	apparelMod::readModel();

	models[0].loadModel( getPathResources("bird.3ds") );
	models[1].loadModel( getPathResources("cat.3ds") );
	models[2].loadModel( getPathResources("dog.3ds") );
	models[3].loadModel( getPathResources("ears.3ds") );

}
/*
void apparelMod_kawaiopathy::drawFaces()
{
}
*/

void apparelMod_kawaiopathy::drawExtra()
{
	if (models[0].getMeshCount()>0)
	{
		ofPushMatrix();
		ofTranslate(-20,20);
		ofRotateZ(180);
		ofScale(0.4f,0.4f,0.4f);
		models[0].getMesh(0).drawWireframe();
		ofPopMatrix();
	}
}
