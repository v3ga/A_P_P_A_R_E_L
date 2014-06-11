//
//  data.h
//  murmur
//
//  Created by Julien on 30/04/13.
//
//

#pragma once
#include "ofMain.h"

#define DATA Data::instance()

class Data
{
    public:
        Data					();
        static Data*			instance();
        void                    load();
 
        ofTrueTypeFont          m_fontGothic;
    
    private:
        static Data*            smp_instance;
};