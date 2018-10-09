#include "EasyBMP/EasyBMP.h"
#include "dcimage.h"
#include "dcencript.cpp"

/*
Modified from:
http://easybmp.sourceforge.net/steganography.html
*/
int encode(const char* secret_filename, const char* image_filename, const char* carrier_filename, const char* key)
{
    // open the input BMP file
    BMP Image;
    Image.ReadFromFile(image_filename);
    int MaxNumberOfPixels = Image.TellWidth() * Image.TellHeight() - 2;
    int k = 1;

    // set image to 32 bpp
    Image.SetBitDepth(32);

    //encript file
    std::string secret_encripted_filename(secret_filename); 
    secret_encripted_filename.append("_encripted");
    endecrypt_file_at(secret_filename, secret_encripted_filename.c_str(), key);

    // open the input text file
    FILE *fp;
    fp = fopen(secret_encripted_filename.c_str(), "rb");
    if (!fp)
    {
        std::cout << "Error: unable to read file " << secret_encripted_filename.c_str() << " for text input!" << std::endl;
        return -1;
    }

    // figure out what we need to encode as an internal header

    EasyBMPstegoInternalHeader IH;
    IH.InitializeFromFile(secret_encripted_filename.c_str(), Image.TellWidth(), Image.TellHeight());
    if (IH.FileNameSize == 0 || IH.NumberOfCharsToEncode == 0)
    {
        return -1;
    }

    // write the internal header;
    k = 0;
    int i = 0;
    int j = 0;
    while (!feof(fp) && k < IH.NumberOfCharsToEncode)
    {
        // decompose the character

        unsigned int T = (unsigned int)IH.CharsToEncode[k];

        int R1 = T % 2;
        T = (T - R1) / 2;
        int G1 = T % 2;
        T = (T - G1) / 2;
        int B1 = T % 2;
        T = (T - B1) / 2;
        int A1 = T % 2;
        T = (T - A1) / 2;

        int R2 = T % 2;
        T = (T - R2) / 2;
        int G2 = T % 2;
        T = (T - G2) / 2;
        int B2 = T % 2;
        T = (T - B2) / 2;
        int A2 = T % 2;
        T = (T - A2) / 2;

        RGBApixel Pixel1 = *Image(i, j);
        Pixel1.Red += (-Pixel1.Red % 2 + R1);
        Pixel1.Green += (-Pixel1.Green % 2 + G1);
        Pixel1.Blue += (-Pixel1.Blue % 2 + B1);
        Pixel1.Alpha += (-Pixel1.Alpha % 2 + A1);
        *Image(i, j) = Pixel1;

        i++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }

        RGBApixel Pixel2 = *Image(i, j);
        Pixel2.Red += (-Pixel2.Red % 2 + R2);
        Pixel2.Green += (-Pixel2.Green % 2 + G2);
        Pixel2.Blue += (-Pixel2.Blue % 2 + B2);
        Pixel2.Alpha += (-Pixel2.Alpha % 2 + A2);
        *Image(i, j) = Pixel2;

        i++;
        k++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }
    }

    // encode the actual data
    k = 0;
    while (!feof(fp) && k < 2 * IH.FileSize)
    {
        char c;
        fread(&c, 1, 1, fp);

        // decompose the character

        unsigned int T = (unsigned int)c;

        int R1 = T % 2;
        T = (T - R1) / 2;
        int G1 = T % 2;
        T = (T - G1) / 2;
        int B1 = T % 2;
        T = (T - B1) / 2;
        int A1 = T % 2;
        T = (T - A1) / 2;

        int R2 = T % 2;
        T = (T - R2) / 2;
        int G2 = T % 2;
        T = (T - G2) / 2;
        int B2 = T % 2;
        T = (T - B2) / 2;
        int A2 = T % 2;
        T = (T - A2) / 2;

        RGBApixel Pixel1 = *Image(i, j);
        Pixel1.Red += (-Pixel1.Red % 2 + R1);
        Pixel1.Green += (-Pixel1.Green % 2 + G1);
        Pixel1.Blue += (-Pixel1.Blue % 2 + B1);
        Pixel1.Alpha += (-Pixel1.Alpha % 2 + A1);
        *Image(i, j) = Pixel1;

        i++;
        k++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }

        RGBApixel Pixel2 = *Image(i, j);
        Pixel2.Red += (-Pixel2.Red % 2 + R2);
        Pixel2.Green += (-Pixel2.Green % 2 + G2);
        Pixel2.Blue += (-Pixel2.Blue % 2 + B2);
        Pixel2.Alpha += (-Pixel2.Alpha % 2 + A2);
        *Image(i, j) = Pixel2;

        i++;
        k++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }
    }

    fclose(fp);
    Image.WriteToFile(carrier_filename);

    return 0;
}

/*
Modified from:
http://easybmp.sourceforge.net/steganography.html
*/
int decode(const char* carrier_filename, const char* key){
    BMP Image;

    Image.ReadFromFile(carrier_filename);
    if (Image.TellBitDepth() != 32)
    {
        std::cout << "Error: File " << carrier_filename << " not encoded with this program." << std::endl;
        return 1;
    }

    EasyBMPstegoInternalHeader IH;
    IH.InitializeFromImage(Image);
    if (IH.FileSize == 0 || IH.FileNameSize == 0 || IH.NumberOfCharsToEncode == 0)
    {
        std::cout << "No hiddent data detected. Exiting ... " << std::endl;
        return -1;
    }

    std::cout << "Hidden data detected! Outputting to file " << IH.FileName << " ... " << std::endl;

    FILE *fp;
    fp = fopen(IH.FileName, "wb");
    if (!fp)
    {
        std::cout << "Error: Unable to open file " << IH.FileName << " for output!\n";
        return -1;
    }

    int MaxNumberOfPixels = Image.TellWidth() * Image.TellHeight();

    int k = 0;
    int i = 0;
    int j = 0;

    // set the starting pixel to skip the internal header
    i = 2 * IH.NumberOfCharsToEncode;
    while (i >= Image.TellWidth())
    {
        i -= Image.TellWidth();
        j++;
    }

    while (k < 2 * IH.FileSize)
    {
        // read the two pixels

        RGBApixel Pixel1 = *Image(i, j);
        i++;
        k++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }

        RGBApixel Pixel2 = *Image(i, j);
        i++;
        k++;
        if (i == Image.TellWidth())
        {
            i = 0;
            j++;
        }

        // convert the two pixels to a character

        unsigned int T = 0;
        T += (Pixel1.Red % 2);
        T += (2 * (Pixel1.Green % 2));
        T += (4 * (Pixel1.Blue % 2));
        T += (8 * (Pixel1.Alpha % 2));

        T += (16 * (Pixel2.Red % 2));
        T += (32 * (Pixel2.Green % 2));
        T += (64 * (Pixel2.Blue % 2));
        T += (128 * (Pixel2.Alpha % 2));

        char c = (char)T;

        fwrite(&c, 1, 1, fp);
    }

    fclose(fp);

    std::string secret_encripted_filename(IH.FileName); 
    secret_encripted_filename.erase(secret_encripted_filename.length()-10);
    endecrypt_file_at(IH.FileName, secret_encripted_filename.c_str(), key);

    return 0;
}