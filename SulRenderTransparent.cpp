// SulRenderTransparent.cpp

#include "stdafx.h"
#include "SulRenderTransparent.h"
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/geode>

CSulRenderTransparent::CSulRenderTransparent( float fBlend ) : 
osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
{
	m_fBlendValue = fBlend;
}

void CSulRenderTransparent::apply( osg::Geode& geode )
{
    osg::StateSet* ss = geode.getOrCreateStateSet();
    osg::BlendFunc* bf = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ss->setMode(GL_BLEND, osg::StateAttribute::ON);
	ss->setAttributeAndModes(bf, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    unsigned pNumDrawables = geode.getNumDrawables();
    for(unsigned i = 0; i < pNumDrawables; ++i)
    {
        osg::Drawable* draw = geode.getDrawable(i);
        osg::Material* mat = dynamic_cast<osg::Material*>(draw->getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL));
        if(mat)
        {
	        mat->setAlpha( osg::Material::FRONT_AND_BACK, m_fBlendValue );
        }
    }
}
