//-----------------------------------
//--- 010 Editor v2.0 Binary Template
//
// File:     BMPTemplate.bt
// Author:   SweetScape Software
// Revision: 2.0
// Purpose:  Defines a template for
//    parsing BMP image files.
//-----------------------------------

// Define structures used in BMP files

typedef struct {   // bmfh
    CHAR    bfType[2];
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct {    // bmih
    DWORD   biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
} BITMAPINFOHEADER;

typedef struct {   // rgbq
    UBYTE   rgbBlue;
    UBYTE   rgbGreen;
    UBYTE   rgbRed;
    UBYTE   rgbReserved;
} RGBQUAD <read=ReadRGBQUAD>;

typedef struct {   // rgbt
    UBYTE   rgbBlue;
    UBYTE   rgbGreen;
    UBYTE   rgbRed;
} RGBTRIPLE <read=ReadRGBTRIPLE>;

//---------------------------------------------
// Custom read functions for color types - this allows the
//  color to be displayed without having to open up the structure.

string ReadRGBQUAD( RGBQUAD &a )
{
    string s;
    SPrintf( s, "#%02X%02X%02X%02X", a.rgbReserved, a.rgbBlue, a.rgbGreen, a.rgbRed );
    return s;
}

string ReadRGBTRIPLE( RGBTRIPLE &a )
{
    string s;
    SPrintf( s, "#%02X%02X%02X", a.rgbBlue, a.rgbGreen, a.rgbRed );
    return s;
}

//---------------------------------------------

// Define the headers
LittleEndian(); 
SetBackColor( cLtGray );
BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;

// Check for header
if( bmfh.bfType != "BM" )
{
    Warning( "File is not a bitmap. Template stopped." );
    return -1;
}

// Define the color table
if( (bmih.biBitCount != 24) && (bmih.biBitCount != 32) )
{
    SetBackColor( cLtAqua );
    if( bmih.biClrUsed > 0 )
        RGBQUAD aColors[ bmih.biClrUsed ];
    else
        RGBQUAD aColors[ 1 << bmih.biBitCount ];
}

// Define the bytes of the data
SetBackColor( cNone );
if( bmih.biCompression > 0 )
{
    // Bytes are compressed
    if( bmih.biSizeImage > 0 )
        UBYTE rleData[ bmih.biSizeImage ];
    else
        UBYTE rleData[ bmfh.bfSize - FTell() ];
}
else
{
    // Calculate bytes per line and padding required
    local int bytesPerLine = (int)Ceil( bmih.biWidth * bmih.biBitCount / 8.0 );
    local int padding      = 4 - (bytesPerLine % 4);
    if( padding == 4 )
        padding = 0;

    // Define each line of the image
    struct BITMAPLINE {

        // Define color data
        if( bmih.biBitCount < 8 )
             UBYTE     imageData[ bytesPerLine ];
        else if( bmih.biBitCount == 8 )
             UBYTE     colorIndex[ bmih.biWidth ];
        else if( bmih.biBitCount == 24 )
             RGBTRIPLE colors[ bmih.biWidth ];
        else if( bmih.biBitCount == 32 )
             RGBQUAD   colors[ bmih.biWidth ];

        // Pad if necessary        
        if( padding != 0 )
             UBYTE padBytes[ padding ];

    } lines[ bmih.biHeight ] <optimize=true>;
} 