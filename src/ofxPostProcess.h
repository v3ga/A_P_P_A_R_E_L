//
//  ofxPostProcess.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 28/08/2014.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxPostProcess : public ofxFXObject {
public:
    ofxPostProcess()
	{
        internalFormat = GL_RGBA;

		fboNoise.allocate(ofGetHeight(),10,GL_RGB);
     
        gl2FragmentShader = STRINGIFY(

								   uniform sampler2DRect 	tex0;
								   uniform sampler2DRect 	tex1;
								   uniform float    		time;
                                   uniform float    		waves;
                                   
                                   void main()
								   {
                                       vec2 st          = gl_TexCoord[0].st;
                                       vec2 st1         = gl_TexCoord[1].st;

										vec2 nnn1		= texture2DRect(tex1, st.yx).xy;
										vec2 nnn2		= texture2DRect(tex1, st.yx+vec2(0.1,0)).xy;
										vec2 nnn3		= texture2DRect(tex1, st.yx+vec2(0.8,0)).xy;

/*                                       vec4 c0 = texture2DRect(tex0,st);
                                       
                                       if(mod(floor(gl_FragCoord.y),waves*5.0)>0.5){
                                           float l = dot(c0.xyz, vec3(0.2126, 0.7152, 0.0722));
                                           gl_FragColor = l * c0;
                                           return;
                                       }

                                       float t = pow((((1.0 + sin(time*10.0) * 0.5)
                                                       * 0.8 + sin(time* cos(gl_FragCoord.y)*41415.92653) * 0.0125)
                                                      * 1.5 + sin(time*7.0) * 0.5), 3.0) + nnn.x*10.0;
*/
									   float t1 = nnn1.x*60.0;
									   float t2 = nnn2.x*60.0;
									   float t3 = nnn3.x*60.0;

                                       vec4 c1 = texture2DRect(tex0, st+vec2(t1*0.9,0.0) );
                                       vec4 c2 = texture2DRect(tex0, st+vec2(t2*0.95,0.0) );
                                       vec4 c3 = texture2DRect(tex0, st+vec2(t3*0.8,0.0) );

                                       vec3 finalCol	= vec3( c1.r, c2.g, c3.b );
                                    
                                       gl_FragColor.rgb = finalCol;
                                       gl_FragColor.a   = 0.5;
                                   }
                                   );

		glESVertexShader = STRINGIFY(

					attribute vec4 position;
					attribute vec4 color;
					attribute vec4 normal;
					attribute vec2 texcoord;

					uniform mat4 modelViewMatrix;
					uniform mat4 projectionMatrix;


						varying vec2 texCoordVarying;

                        void main()
						{
							texCoordVarying = texcoord;
                            gl_Position = projectionMatrix * modelViewMatrix * position;
						}
									);


        glESFragmentShader = STRINGIFY(
								   precision highp float;

								   uniform sampler2D	 	tex0;
								   uniform sampler2D	 	tex1;
								   
								   varying vec2 texCoordVarying;

                                   void main()

								   {

                                       gl_FragColor.x = texCoordVarying.x/960.0;
                                       gl_FragColor.y = texCoordVarying.y/640.0;
/*                                       gl_FragColor.x = texCoordVarying.x/960.0;
                                       gl_FragColor.y = texCoordVarying.y/640.0;
	                                   gl_FragColor.z = 0.0;
	                                   gl_FragColor.a = 1.0;
*/
	                                   gl_FragColor = texture2D(tex0,vec2(texCoordVarying.x/640.0,texCoordVarying.y/960.0));

										   }
                                   );


    }
	
	void noise()
	{
		// Time
		float time = ofGetElapsedTimef();

		// Generate noise
		fboNoise.begin();
		ofClear(255);
		for (float x=0; x<=fboNoise.getWidth(); x++)
		{
			ofSetColor( 255*ofNoise(x*0.98+time),255 );
			ofLine(x,0,x,10);
		}
		fboNoise.end();
	
	}
    
    void update()
	{
		// Time
		float time = ofGetElapsedTimef();
		noise();
	
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniformTexture( "tex1" , fboNoise.getTextureReference(), 1 );
        shader.setUniform1f		( "time" , time );
        shader.setUniform1f		( "waves" , 3.0f );
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
 
	
	ofFbo		fboNoise;
};
