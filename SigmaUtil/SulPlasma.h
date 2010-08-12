// SulPlasma.h

#ifndef __SULPLASMA_H__
#define __SULPLASMA_H__

#include "SulTypes.h"
#include "SulExport.h"
#include <osg/Referenced>

class SUL_EXPORT CSulPlasma : public osg::Referenced
{
public:
								CSulPlasma();
								~CSulPlasma();

	void						Create( sigma::uint32 w, sigma::uint32 h );
	sigma::uint8				GetValue( sigma::uint32 dwPosX, sigma::uint32 dwPosY );
	sigma::uint8*				GetDataPtr();

private:
	int							NewColor( sigma::int32 mc, sigma::int32 n, sigma::int32 dvd );
	void						Plasma( sigma::int32 x1, sigma::int32 y1, sigma::int32 x2, sigma::int32 y2, sigma::uint8* pData );

private:
	sigma::uint32				m_maxX;
	sigma::uint32				m_maxY;
	sigma::uint8*				m_pData;
};

#endif // __SULPLASMA_H__
