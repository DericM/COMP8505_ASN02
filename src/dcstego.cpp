/*
Deric Mccadden
October 8, 2018
*/

#include "EasyBMP/EasyBMP.h"
#include <cstdio>
#include <cstdlib>
#include "dcimage.cpp"
#include "dcutils.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    bool EncodeMode = false;
    bool DecodeMode = false;
    bool HelpMode = false;

    int k = 1;
    for (k = 1; k < argc; k++)
    {
        if (*(argv[k] + 0) == '-')
        {
            if (*(argv[k] + 1) == 'h')
            {
                HelpMode = true;
            }
            if (*(argv[k] + 1) == 'd')
            {
                DecodeMode = true;
            }
            if (*(argv[k] + 1) == 'e')
            {
                EncodeMode = true;
            }
        }
    }

    if (DecodeMode && EncodeMode)
    {
        HelpMode = true;
    }
    if (!DecodeMode && !EncodeMode)
    {
        HelpMode = true;
    }
    if (EncodeMode && argc < 6)
    {
        HelpMode = true;
    }
    if (DecodeMode && argc < 4)
    {
        HelpMode = true;
    }
    if(argc == 1)
    {
        HelpMode = true;
    }

    if (HelpMode)
    {
        cout << "Embed Text: \n"
             << "          Stego -e <secret file> <input bitmap> <output bitmap> <encript key>\n"
             << "Extract Text: \n"
             << "          Stego -d <input bitmap> <decript key>\n"
             << "Help: \n"
             << "          Stego -h\n\n";
        return 1;
    }

    if (EncodeMode)
    {
        if (encode(argv[2], argv[3], argv[4], argv[5]) == -1)
        {
            return -1;
        }
    }
    if (DecodeMode)
    {
        if(decode(argv[2], argv[3]) == -1){
            return -1;
        }
    }

    return 0;
}
