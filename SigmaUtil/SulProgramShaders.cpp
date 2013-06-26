// SulProgramShaders.cpp

#include "stdafx.h"
#include "SulProgramShaders.h"
#include "SulStringList.h"
#include <osgDB/FileUtils>

CSulProgramShaders::CSulProgramShaders( CSulString sShaderFiles )
{
	CSulStringList list;
	list.explode( sShaderFiles );

	const CSulStringList::VECTOR_STRING& vec = list.getList();

	CSulStringList::VECTOR_STRING::const_iterator is = vec.begin();
	CSulStringList::VECTOR_STRING::const_iterator ie = vec.end();

	while ( is!=ie )
	{
		CSulString s = (*is);					// complete file name
		CSulString ext = s.getExtension();		// extension
		ext.makeLower();

		osg::ref_ptr< osg::Shader > shader = new osg::Shader();
		shader->setName( s );

		if ( ext=="vert" )
			shader->setType( osg::Shader::VERTEX );
		else if ( ext=="frag" )
			shader->setType( osg::Shader::FRAGMENT );
		else if ( ext=="geom" )
			shader->setType( osg::Shader::GEOMETRY );

		if ( !shader->loadShaderSourceFromFile( osgDB::findDataFile(s) ) )
		{
			osg::notify(osg::ALWAYS) << "WARNING: CSulProgramShaders.cpp -> could not load: " << s << std::endl;
		}
				
		addShader( shader );

		++is;
	}

}