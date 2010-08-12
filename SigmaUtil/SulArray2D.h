// SulArray2D.h

#ifndef __SULARRAY2D_H__
#define __SULARRAY2D_H__

#include "SulTypes.h"
#include <osg/Referenced>

template<typename _T>
class CSulArray2D : public osg::Referenced
{
public:
	CSulArray2D( sigma::uint32 w, sigma::uint32 h )
	{
		m_data = new _T[w*h*sizeof(_T)];
		m_w = w;
		m_h = h;
		memset( m_data, 0, w*h*sizeof(_T) );
	}

	void Set( sigma::uint32 x, sigma::uint32 y, _T val )
	{
		if ( x>=m_w ) x = m_w-1;
		if ( y>=m_h ) y = m_h-1;
		//m_data[x*sizeof(_T)+y*(m_w*sizeof(_T))] = val;
		m_data[x+y*m_w] = val;
	}

	_T Get( sigma::uint32 x, sigma::uint32 y )
	{
		//return m_data[x*sizeof(_T)+y*(m_w*sizeof(_T))];
		return m_data[x+y*m_w];
	}

	sigma::uint32 GetWidth()
	{
		return m_w;
	}

	sigma::uint32 GetHeight()
	{
		return m_h;
	}

	_T* GetDataPtr()
	{
		return m_data;
	}

private:
	~CSulArray2D()
	{
		// FIXME: why can't I delete here
	//	delete[] m_data;
	}

private:
	_T*				m_data;
	sigma::uint32	m_w;
	sigma::uint32	m_h;
};

#endif // __SULARRAY2D_H__