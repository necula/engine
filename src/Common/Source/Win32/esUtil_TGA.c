//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// esUtil_TGA.c
//
//    This file contains the Win32 implementation of a TGA image loader

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifdef OS_ANDROID

	/* Bitmap Header Definition */
	typedef struct tagBITMAP
	{
		long int        bmType;
		long int        bmWidth;
		long int        bmHeight;
		long int        bmWidthBytes;
		int        bmPlanes;
		int        bmBitsPixel;
		void*      bmBits;
	} BITMAP, *PBITMAP;

	typedef struct tagRGBTRIPLE {
        unsigned char    rgbtBlue;
        unsigned char    rgbtGreen;
        unsigned char    rgbtRed;
	} RGBTRIPLE, *PRGBTRIPLE;

	typedef struct tagRGBQUAD {
			unsigned char    rgbBlue;
			unsigned char    rgbGreen;
			unsigned char    rgbRed;
			unsigned char    rgbReserved;
	} RGBQUAD;
	typedef RGBQUAD;

#endif



///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

///
//  Types
//

#ifdef _WIN32

#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)

typedef struct
{
   unsigned char  IdSize,
                  MapType,
                  ImageType;
   unsigned short PaletteStart,
                  PaletteSize;
   unsigned char  PaletteEntryDepth;
   unsigned short X,
                  Y,
                  Width,
                  Height;
   unsigned char  ColorDepth,
                  Descriptor;
         
} TGA_HEADER;

#pragma pack(pop,x1)

#endif

#ifdef OS_ANDROID

typedef struct
{
   unsigned char  IdSize,
                  MapType,
                  ImageType;
   unsigned short PaletteStart,
                  PaletteSize;
   unsigned char  PaletteEntryDepth;
   unsigned short X,
                  Y,
                  Width,
                  Height;
   unsigned char  ColorDepth,
                  Descriptor;
         
} __attribute__((__packed__)) TGA_HEADER;

#endif

////////////////////////////////////////////////////////////////////////////////////
//
//  Private Functions
//

////////////////////////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  WinTGALoad()
//
int WinTGALoad( const char *fileName, char **buffer, int *width, int *height, int *colorDepth )
{
   FILE        *fp;
   TGA_HEADER   Header;
	
   fp = fopen(fileName, "rb");
   /*
   if ( fopen_s ( &fp, fileName, "rb" ) != 0 )
   {
      return FALSE;
   }
   */

   if ( fp == NULL )
   {
      return FALSE;
   }

   fread ( &Header, sizeof(TGA_HEADER), 1, fp );

   *width = Header.Width;
   *height = Header.Height;
   *colorDepth = Header.ColorDepth;
   
   if ( Header.ColorDepth == 24 )
   {
      RGBTRIPLE *Buffer24;

      Buffer24= (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE) * (*width) * (*height));

      if(Buffer24)
      {
         int i=0;
         int x,
             y;

         fread(Buffer24, sizeof(RGBTRIPLE), (*width) * (*height), fp);


		 *buffer= (char*) malloc(3 * (*width) * (*height));

         for ( y = 0; y < *height; y++ )
            for( x = 0; x < *width; x++ )
            {
               int Index= y * (*width) + x;

               if(!(Header.Descriptor & INVERTED_BIT))
                  Index= ((*height) - 1 - y) * (*width) + x;

               (*buffer)[(i * 3)]=      Buffer24[Index].rgbtRed;
               (*buffer)[(i * 3) + 1]=  Buffer24[Index].rgbtGreen;
               (*buffer)[(i * 3) + 2]=  Buffer24[Index].rgbtBlue;
        
               i++;
            }
         
         fclose(fp);
         free(Buffer24);

         return(TRUE);
      }		
   }
   else if ( Header.ColorDepth == 32 )
   {
      RGBQUAD *Buffer32;

      Buffer32= (RGBQUAD*)malloc(sizeof(RGBQUAD) * (*width) * (*height));

      if(Buffer32)
      {
         int i=0;
         int x,
             y;

         fread(Buffer32, sizeof(RGBQUAD), (*width) * (*height), fp);

		 *buffer= (char*) malloc(4 * (*width) * (*height));


         for ( y = 0; y < *height; y++ )
            for( x = 0; x < *width; x++ )
            {
               int Index= y * (*width) + x;

               //if(!(Header.Descriptor & INVERTED_BIT))
                  //Index= ((*height) - 1 - y) * (*width) + x;

               (*buffer)[(i * 4)]=      Buffer32[Index].rgbRed;
               (*buffer)[(i * 4) + 1]=  Buffer32[Index].rgbGreen;
               (*buffer)[(i * 4) + 2]=  Buffer32[Index].rgbBlue; 
			   (*buffer)[(i * 4) + 3]=  Buffer32[Index].rgbReserved;
               i++;
            }
         
         fclose(fp);
         free(Buffer32);
         return(TRUE);
      }		
   }

   return(FALSE);
}

//#endif