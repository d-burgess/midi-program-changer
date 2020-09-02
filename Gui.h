/*

Gui

*/

#ifndef __MPCGui__
#define __MPCGui__

#include "LcdBuffer.h"
#include "Logger.h"
#include "GuiElement.h"
#include <stdio.h>
#include <cstring>

enum mpcMode { play, freeze, thru, save, edit };

class Gui {
    private:
        LcdBuffer * pLcdBuffer;
        Logger * pLogger;
        uint8_t numElements = 0;
        GuiElement * element[ 10 ];
        uint8_t selectedElement = 1;
        void padByteWithLeadingZeros( char * ref, uint8_t aByte, uint8_t totalLength );
        mpcMode currentMode;
    public:
        explicit Gui( LcdBuffer * lcdBuffer, Logger * logger );
        ~Gui();
        void addElement( GuiElement * e );
        uint8_t elementCount();
        void update();
        void incrementElement( uint8_t e );
        void incrementSelectedElement();
        void decrementSelectedElement();
        void selectNextElement();
        void setCursorToCurrentElement();
        void cycleMode();
        mpcMode getCurrentMode();
};

#endif
