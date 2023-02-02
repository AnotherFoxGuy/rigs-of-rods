project(OgreWater)

include(SourceFileUtils)

set(SOURCE_FILES
        OWAnimatedMesh.{h,cpp}
        OWWater.{h,cpp}
        SimplexNoise.{h,cpp}
        )

expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

add_library(OgreWater STATIC ${SOURCE_FILES})

target_include_directories(OgreWater PRIVATE .)
target_link_libraries(OgreWater PRIVATE OGRE::OGRE)