#include "EasyBMP/EasyBMP.h"

#ifndef DCIMAGE
#define DCIMAGE

/*
Taken from:
http://easybmp.sourceforge.net/steganography.html
*/
class EasyBMPstegoInternalHeader
{
  public:
    char *FileName;
    int FileNameSize;

    int FileSize;

    unsigned char *CharsToEncode;
    int NumberOfCharsToEncode;

    void InitializeFromFile(const char *input, int BMPwidth, int BMPheight);
    void InitializeFromImage(BMP &Input);
};

#endif