// SulUniformArrayHack.h

#ifndef __SULUNIFORMARRAYHACK_H__
#define __SULUNIFORMARRAYHACK_H__

/*
	This is a hack fix for the bug in the driver/osg when using uniform arrays... the uniform name doesn't map correctly, because
	of the "[0]" being added or removed with different drivers.

	This hack creates two uniforms for each definition
	One with "name[0]" and one just with "name", this works fine because the shader itself only references the "name" with index
*/

#include "SulTypes.h"
#include "SulString.h"
#include "SulExport.h"
#include <osg/uniform>
#include <osg/referenced>

class SUL_EXPORT CSulUniformArrayHack : public osg::Referenced
{
public:
	CSulUniformArrayHack( const CSulString& name, sigma::int32 numElements, osg::Uniform::Type type, osg::StateSet* ssToReceive );
	
	void setElement( sigma::int32 index, const osg::Vec3& value );
	void setElement( sigma::int32 index, const osg::Vec4& value );
	void setElement( sigma::int32 index, float value );
	void setElement( sigma::int32 index, sigma::int32 value );
	void setElement( sigma::int32 index, bool value );

private:
	osg::ref_ptr<osg::Uniform> m_u1;
	osg::ref_ptr<osg::Uniform> m_u2;
};

#endif // __SULUNIFORMARRAYHACK_H__