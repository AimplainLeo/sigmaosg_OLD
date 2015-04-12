# Introduction #
||Using the GPU instancing we can create thousands of trees with very little cpu usage. This application generates a texturemap that will contain the positions for all trees based on the given shapefile. The application can also remove trees from the "position texture" based on shapefiles used as masks. For example a shapefile that describes the road positions is well suited for this, we don't want trees in the middle of the road.|<a href='http://www.youtube.com/watch?feature=player_embedded&v=Mmhd8UtP0l8' target='_blank'><img src='http://img.youtube.com/vi/Mmhd8UtP0l8/0.jpg' width='425' height=344 /></a>|
|:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# Details #

An IVE file is created which contain all trees and shaders needed to render these trees. All you have to do is include the IVE into your scene.

# Xml Settings #

```
<!-- General Setup Parameters 

  useLights : hex value i.e. "00000001" or "00000002" or "00000004"...
  col_geom : if "true" will create invisible collision geometry for trees
  no_geo : if "true" then geo position from object models is not used (i.e. .osg .ive files do not contain geo position data, but flt does)
  areapadding : ???? (hehe, don't remember what this does,.. something with padding)
  suppress_shaders : if "true" the output file will not have shader code include
-->

<GEN
    useLights="00000001"
    col_geom="true"
    no_geo="true"
    areapadding="1"
    suppress_shader="false"
  >

  <!-- Texture of trees  -->
  <!-- Suppress : "true" will not copy texture to all blocks  -->
  <!-- unit : x texture unit to lookup texture -->
  <TEXTURE_TREE 
    suppress="true"
    unit="4"
  />
  
  <!-- Parameters for the trees  -->
  <!-- Everthing is in ref to 1 meter -->
  <!-- "Radius" dist between trees-->
  <TREE 
    sizemultiplier="40.0" 
    radius_between_trees="2.0"
  />
  
  <TERRAIN 
    file="\\tvbuild\FACTerrain\FACTerrain\FACTerrain0_0.flt"
    offsetx="10240"
    offsety="10240"
  />
  
  <!-- Path to file with tree area and lines -->
  <!-- offsetz and renderme is for debug -->
  <SHAPE 
    file="\\tvbuild\FACTerrain\convert\trees_all.flt"
    offsetz="100"
    renderme="false"
  />

	<!-- Path to cultures that should mask out trees --> 
	<!-- linedist=x buffer zone between culture and trees   --> 
	<!-- offsetz and renderme is for debug -->
  <MASK file="\\tvbuild\FACTerrain\convert\byg.flt" linedist="4" offsetz="150"/>
  <MASK file="\\tvbuild\FACTerrain\convert\b.flt" linedist="4" offsetz="150"/>
  
  <MASK file="\\tvbuild\FACTerrain\convert\vm.flt" linedist="4" offsetz="150"/>
  <MASK file="\\tvbuild\FACTerrain\convert\v6.flt" linedist="4" offsetz="150" renderme="true"/>

  <MASK file="\\tvbuild\FACTerrain\convert\l.flt" linedist="4" offsetz="150" renderme="true"/>
  
  <MASK file="\\tvbuild\FACTerrain\convert\v3_5.flt" linedist="4" offsetz="150" renderme="true"/>
  <MASK file="\\tvbuild\FACTerrain\convert\av.flt" linedist="4" offsetz="150"/>
  <MASK file="\\tvbuild\FACTerrain\convert\vindmoeller.flt" linedist="4" offsetz="150"/>
 
	<!-- Not used when run with masterconv --> 
  <OUTPUT file="trees.ive" />
  
  <VIEWER suppress="false" />
  
  <DEBUGGING 
    showpivots="false"
  />
    
</GEN>




```