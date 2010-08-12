// plasma.cpp

#include "stdafx.h"
#include "SulPlasma.h"

#define MAX_H 255
#define MIN_H 1

CSulPlasma::CSulPlasma()
{
	m_pData = 0;
}

CSulPlasma::~CSulPlasma()
{
	if ( m_pData )
	{
		delete[] m_pData;
	}
}


int CSulPlasma::NewColor( int mc, int n, int dvd )
{
	sigma::int32 loc;

	loc = ( mc + n - (2*n*rand())/RAND_MAX ) / dvd;
	
	if ( loc>MAX_H )
		return  MAX_H;

	if ( loc<MIN_H )
		return MIN_H;

	return loc;
}


void CSulPlasma::Plasma( int x1, int y1, int x2, int y2, sigma::uint8* pData )
{
	sigma::int32 xn, yn, dxy, p1, p2, p3, p4;

	if ( (x2-x1<2) && (y2-y1<2) )
		return;

	p1 = pData[m_maxX*y1+x1];
	p2 = pData[m_maxX*y2+x1];
	p3 = pData[m_maxX*y1+x2];
	p4 = pData[m_maxX*y2+x2];


	xn = (x2+x1) >> 1;
	yn = (y2+y1) >> 1;

	dxy = 10*(x2-x1+y2-y1) /3;

	if ( pData[m_maxX*y1+xn]==0 ) pData[m_maxX*y1+xn] = (unsigned char)NewColor( p1+p3, dxy, 2 );
	if ( pData[m_maxX*yn+x1]==0 ) pData[m_maxX*yn+x1] = (unsigned char)NewColor( p1+p2, dxy, 2 );
	if ( pData[m_maxX*yn+x2]==0 ) pData[m_maxX*yn+x2] = (unsigned char)NewColor( p3+p4, dxy, 2 );
	if ( pData[m_maxX*y2+xn]==0 ) pData[m_maxX*y2+xn] = (unsigned char)NewColor( p2+p4, dxy, 2 );

	pData[m_maxX*yn+xn] = (sigma::int8)NewColor( p1+p2+p3+p4, dxy, 4 );

	Plasma( x1, y1, xn, yn, pData ); 
	Plasma( xn, y1, x2, yn, pData );
	Plasma( x1, yn, xn, y2, pData ); 
	Plasma( xn, yn, x2, y2, pData );
}


void CSulPlasma::Create( sigma::uint32 w, sigma::uint32 h )
{
	if ( m_pData )
	{
		delete[] m_pData;
	}

	m_maxX = w;
	m_maxY = h;
	
	m_pData = new sigma::uint8[w*h*sizeof(sigma::uint8)];

	memset( m_pData, 0, w*h*sizeof(sigma::int8) );

	Plasma( 0, 0, w-1, h-1, m_pData );
}

sigma::uint8 CSulPlasma::GetValue( sigma::uint32 dwPosX, sigma::uint32 dwPosY )
{
	if ( m_pData )
	{
		return m_pData[dwPosX+dwPosY*m_maxY];
	}

	return 0;
}

sigma::uint8* CSulPlasma::GetDataPtr()
{
	return m_pData;
}
