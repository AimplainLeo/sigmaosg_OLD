// SulAnimationPath.h

#ifndef __SULANIMATIONPATH_H__
#define __SULANIMATIONPATH_H__

#include "SulAnimationPathFunctor.h"
#include "SulExport.h"
#include <osg/NodeCallback>
#include <osg/AnimationPath>

class SUL_EXPORT CSulAnimationPathCallback : public osg::NodeCallback
{
public:
	enum EMODE
	{
		MODE_STOP,
		MODE_PLAY_FORWARD,
		MODE_PLAY_REVERSE
	};

	enum EACTION
	{
		ACTION_UNKNOWN,
		ACTION_END
	};

private:
	typedef std::multimap< EACTION, osg::ref_ptr<CSulAnimationPathFunctorBase> >	MULTIMAP_ANIPATHCALLBACK;

public:
										CSulAnimationPathCallback();

	void								SetPath( osg::AnimationPath* path );

	void								SetMode( EMODE eMode );

	void								WantCallback( CSulAnimationPathFunctorBase* pFunctor, EACTION eAction=ACTION_END );

private:
	void								operator()( osg::Node* node, osg::NodeVisitor* nv );

	double								GetAnimationTime();

	void								Update( osg::Node& node );
	void								UpdateCallbacks();

private:
	osg::ref_ptr<osg::AnimationPath>	m_animationPath;
	osg::Vec3d							m_pivotPoint;
	EMODE								m_eMode;

	double								m_latestTime;
	double								m_time;
	double								m_lastTime;

	MULTIMAP_ANIPATHCALLBACK			m_multimapAniPathCallback;
	bool								m_bNotified;
};

#endif // __SULANIMATIONPATH_H__