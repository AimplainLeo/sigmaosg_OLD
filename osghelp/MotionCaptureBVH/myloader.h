// myloader.h

#ifndef __MYLOADER_H__
#define __MYLOADER_H__

#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/Registry>

#include <osgAnimation/Bone>
#include <osgAnimation/Animation>

class myBvhMotionBuilder : public osg::Referenced
{
public:
	typedef std::vector<int>	VEC_CHANNEL;

	typedef std::pair<osg::ref_ptr<osgAnimation::Bone>, VEC_CHANNEL> JointNode;
    typedef std::vector<JointNode> JointList;

    myBvhMotionBuilder();
    virtual ~myBvhMotionBuilder();

    static myBvhMotionBuilder* instance()
    {
        static osg::ref_ptr<myBvhMotionBuilder> s_library = new myBvhMotionBuilder;
        return s_library.get();
    }

    void buildHierarchy( osgDB::Input& fr, int level, osgAnimation::Bone* parent );

    void buildMotion( osgDB::Input& fr, osgAnimation::Animation* anim );

    osg::Group* buildBVH( std::istream& stream, const osgDB::ReaderWriter::Options* options );

protected:
    void alterChannel( std::string name, VEC_CHANNEL& value );

	void setKeyframe( osgDB::Input& fr, VEC_CHANNEL& ch, double time,
                      osgAnimation::Vec3KeyframeContainer* posKey,
                      osgAnimation::QuatKeyframeContainer* rotKey );

	osg::ref_ptr<osg::Geode> createRefGeometry( osg::Vec3 p, double len );

    int _drawingFlag;
    JointList _joints;
};

class myReaderWriterBVH : public osgDB::ReaderWriter
{
public:
    myReaderWriterBVH()
    {
        supportsExtension( "bvh2", "Biovision motion hierarchical file" );

        supportsOption( "contours","Show the skeleton with lines." );
        supportsOption( "solids","Show the skeleton with solid boxes." );
    }

    virtual const char* className() const
    { return "BVH Motion Reader"; }

    virtual ReadResult readNode(std::istream& stream, const osgDB::ReaderWriter::Options* options) const
    {
        ReadResult rr = myBvhMotionBuilder::instance()->buildBVH( stream, options );
        return rr;
    }

    virtual ReadResult readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
    {
        std::string ext = osgDB::getLowerCaseFileExtension( file );
        if ( !acceptsExtension(ext) ) return ReadResult::FILE_NOT_HANDLED;

        std::string fileName = osgDB::findDataFile( file, options );
        if ( fileName.empty() ) return ReadResult::FILE_NOT_FOUND;

        osgDB::ifstream stream( fileName.c_str(), std::ios::in|std::ios::binary );
        if( !stream ) return ReadResult::ERROR_IN_READING_FILE;
        return readNode( stream, options );
    }
};

#endif // __MYLOADER_H__