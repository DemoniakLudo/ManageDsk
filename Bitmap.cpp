#include  <afxwin.h>

#include  "Bitmap.h"


BOOL MonBitmap::Create( HDC hDC, int tx, int ty )
{
    if ( ! Created )
        {
        BITMAPINFO pbmi;

        TailleX = tx;
        TailleY = ty;
        pbmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
        pbmi.bmiHeader.biWidth = TailleX;
        pbmi.bmiHeader.biHeight = -TailleY;
        pbmi.bmiHeader.biPlanes = 1;
        pbmi.bmiHeader.biBitCount = 32;
        pbmi.bmiHeader.biCompression = BI_RGB;
        pbmi.bmiHeader.biSizeImage = 0;
        pbmi.bmiHeader.biXPelsPerMeter = 0;
        pbmi.bmiHeader.biYPelsPerMeter = 0;
        pbmi.bmiHeader.biClrUsed = 0;
        pbmi.bmiHeader.biClrImportant = 0;
        bm = CreateDIBSection( hDC
                             , &pbmi
                             , DIB_RGB_COLORS
                             , ( void ** )&MemBitmap
                             , NULL
                             , 0
                             );
        if ( bm )       
            {
            DcBitmap = CreateCompatibleDC( hDC );
            if ( DcBitmap )
                {
                SelectObject( DcBitmap, bm );
                SetStretchBltMode( DcBitmap, COLORONCOLOR );
                Created = TRUE;
                }
            }
        }
    return( Created );
}


void MonBitmap::Remove( void )
{
    if ( DcBitmap )
        DeleteDC( DcBitmap );

    if ( bm )
        DeleteObject( bm );

    DcBitmap = NULL;
    bm = NULL;
}


void MonBitmap::Render( HDC DcDest, int PosX, int PosY, int MulX, int MulY )
{
    StretchBlt( DcDest
          , PosX, PosY, TailleX * MulX, TailleY * MulY
          , DcBitmap, 0, 0, TailleX, TailleY
          , SRCCOPY
          );
}