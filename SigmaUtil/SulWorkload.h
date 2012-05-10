// SulWorkload.h

#ifndef __SULWORKLOAD_H__
#define __SULWORKLOAD_H__

#include <osg/timer>
#include <osg/ShapeDrawable>

/*

How to use:




*/


class SimulatedWorkload
{
public:
	SimulatedWorkload(int workloadTimeMS) : _workloadTimeMS(workloadTimeMS) {}

protected:
	void simulateWorkload() const
	{
		osg::Timer_t start = osg::Timer::instance()->tick();
		while (osg::Timer::instance()->delta_m(start, osg::Timer::instance()->tick()) < _workloadTimeMS) {}
	}

	int _workloadTimeMS;
};

class SimulatedWorkloadCallback : public osg::NodeCallback, public SimulatedWorkload
{
public:
	SimulatedWorkloadCallback(int workloadTimeMS) : SimulatedWorkload(workloadTimeMS) {}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		simulateWorkload();
		traverse(node,nv);
	}
};

class SimulatedWorkloadDrawCallback : public osg::Drawable::DrawCallback, public SimulatedWorkload
{
public:
	SimulatedWorkloadDrawCallback(int workloadTimeMS) : SimulatedWorkload(workloadTimeMS) {}

	virtual void drawImplementation(osg::RenderInfo& /*renderInfo*/,const osg::Drawable* /*drawable*/) const
	{
		simulateWorkload();
	}
};

class CSulWorkload
{
public:
	CSulWorkload( osg::Group* group, float msDrawCallback=20.0f, float msCullCallback=10.0f, float msUpdateCallback=10.0f )
	{
		osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), 10.f));
		sd->setDrawCallback(new SimulatedWorkloadDrawCallback(msDrawCallback));
		sd->setUseDisplayList(false);
		osg::Geode* geode = new osg::Geode;
		geode->addDrawable(sd);
		geode->setCullingActive(false);
		group->addChild( geode );
		group->setCullCallback(new SimulatedWorkloadCallback(msCullCallback));
		group->setUpdateCallback(new SimulatedWorkloadCallback(msUpdateCallback));
	}
};

#endif // __SULWORKLOAD_H__