//
//  apparelMode_porcupinopathy.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 05/12/2014.
//
//

#include "apparelMood_porcupinopathy.h"
#include "ofAppLog.h"

apparelMood_porcupinopathy::apparelMood_porcupinopathy() : apparelMod("Porcupinopathy")
{
	mp_meshSubdivided = 0;
}

apparelMood_porcupinopathy::~apparelMood_porcupinopathy()
{
	deleteMeshSubdivided();
}


void apparelMood_porcupinopathy::apply()
{
	if (isChanged())
	{
		OFAPPLOG->begin("apparelMood_porcupinopathy::apply()");
	
		m_model.mesh = m_meshInput;
	
		deleteMeshSubdivided();

		int nbFacesSelected = m_indicesFaces.size();
		if (nbFacesSelected>0)
		{
		
			mp_meshSubdivided = new ofMesh();
			mp_meshSubdivided->setMode(OF_PRIMITIVE_TRIANGLES);
		
			for (int i=0; i<nbFacesSelected; i++)
			{
			 	ofMeshFaceApparel* pFace = m_model.getMeshFacesRef()[ m_indicesFaces[i] ];
				mp_meshSubdivided->addVertex( pFace->getVertex(0) );
				mp_meshSubdivided->addVertex( pFace->getVertex(1) );
				mp_meshSubdivided->addVertex( pFace->getVertex(2) );

				mp_meshSubdivided->addNormal(pFace->getNormal(0));
				mp_meshSubdivided->addNormal(pFace->getNormal(1));
				mp_meshSubdivided->addNormal(pFace->getNormal(2));


				mp_meshSubdivided->addIndex(i*3+0);
				mp_meshSubdivided->addIndex(i*3+1);
				mp_meshSubdivided->addIndex(i*3+2);
				


			}

		  //  butterfly.topology_start(*mp_meshSubdivided);
        
    		//butterfly.topology_subdivide_butterfly();
			m_meshFinal = butterfly.subdivideLinear(*mp_meshSubdivided);
//			m_meshFinal = butterfly.subdivideLinear(m_meshFinal);
//			m_meshFinal = butterfly.subdivideLinear(m_meshFinal);

//			OFAPPLOG->println(" - created mesh with "+ofToString(nbFacesSelected)+ "face(s)");

/*
			ofVec3f middleFace;
			ofVec3f normalFace;
			ofVec3f A,B,C;
			const vector<ofMeshFace>& meshFinalFaces = m_meshFinal.getUniqueFaces();
			int nbFaces = meshFinalFaces.size();
			for (int i=0;i<nbFaces;i++)
			{
				const ofMeshFace& face = meshFinalFaces[i];
				C = middleFace + normalFace*30.0f;
				for (int j=0; j<3 ; j++)
				{
				  	A = face.getVertex(j);
				  	B = face.getVertex((j+1)%3);
//					ofTriangle(A,B,C);
					
					m_model.mesh.addVertex(A);
					m_model.mesh.addVertex(B);
					m_model.mesh.addVertex(C);

					m_model.mesh.addIndex( m_model.mesh.getNumIndices() );
					m_model.mesh.addIndex( m_model.mesh.getNumIndices() );
					m_model.mesh.addIndex( m_model.mesh.getNumIndices() );
					
				}
			}
*/
		}

		OFAPPLOG->end();
	}
}


void apparelMood_porcupinopathy::drawExtra()
{
/*	if (mp_meshSubdivided)
	{
		ofPushStyle();
		ofSetColor(0,255,0);
		ofPushMatrix();
		ofMultMatrix(m_model.getModelMatrix());
//		float s = 1.005;
//		ofScale(s,s,s);
//		m_meshFinal.drawWireframe();
		const vector<ofMeshFace>& meshFinalFaces = m_meshFinal.getUniqueFaces();
		int nbFaces = meshFinalFaces.size();
		ofVec3f middleFace;
		ofVec3f normalFace;
		ofVec3f A,B,C;
		ofNoFill();
		for (int i=0;i<nbFaces;i++)
		{
			const ofMeshFace& face = meshFinalFaces[i];
			middleFace 	= (face.getVertex(0) + face.getVertex(1) + face.getVertex(2))/3;
			normalFace = face.getFaceNormal();

			C = middleFace + normalFace*30.0f;
			for (int j=0; j<3 ; j++)
			{
			  	A = face.getVertex(j);
			  	B = face.getVertex((j+1)%3);
				ofTriangle(A,B,C);
			}
			ofLine(middleFace,C);



		}
		ofPopMatrix();
		ofPopStyle();
	}
*/
}


void apparelMood_porcupinopathy::deleteMeshSubdivided()
{
	if (mp_meshSubdivided)
		delete mp_meshSubdivided;
	mp_meshSubdivided = 0;
}
