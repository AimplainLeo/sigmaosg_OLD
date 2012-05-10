// SulLightBase.h

#ifndef __SULLIGHTBASE__H__
#define __SULLIGHTBASE__H__

#include "SulTypes.h"
#include "SulExport.h"
#include <osg/referenced>

class CSulLightManager;

class SUL_EXPORT CSulLightBase : public osg::Node
{
public:
							CSulLightBase( CSulLightManager* lm );

	void					setAmbientColor( const osg::Vec4& color );
	const osg::Vec4&		getAmbientColor() const;

	void					setDiffuseColor( const osg::Vec4& color );
	const osg::Vec4&		getDiffuseColor() const;

	void					setId( sigma::int32 id );
	sigma::int32			getId();

	void					setDirtyFlag( bool dirty );
	bool					isDirty() const;

	void					setEnabled( bool enabled );
	bool					isEnabled() const;

	// local space
	void					setPosition( const osg::Vec3& pos );
	const osg::Vec3&		getPosition() const;

	// view space
	void					setViewPosition( const osg::Vec3& pos );
	const osg::Vec3&		getViewPosition();

	void					setConstantAtt( float att );
	void					setLinearAtt( float att );
	void					setQuadraticAtt( float att );

	float					getConstantAtt() const;
	float					getLinearAtt() const;
	float					getQuadraticAtt() const;

	virtual void			traverse( osg::NodeVisitor& nv );

private:
	virtual osg::BoundingSphere	computeBound() const;

protected:
	CSulLightManager*		m_lm;
	float					m_attConstant;
	float					m_attLinear;
	float					m_attQuadratic;
	osg::Vec3				m_posViewMatrix;
	osg::Vec3				m_pos;
	bool					m_dirty;
	bool					m_enabled;
	sigma::int32			m_id;
	osg::Vec4				m_diffuseColor;
	osg::Vec4				m_ambientColor;
};

#endif // __SULLIGHTBASE__H__