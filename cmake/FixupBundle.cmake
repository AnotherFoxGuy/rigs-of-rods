include(BundleUtilities)
include(${BIN_DIR}/cmake/ConanBinDirs.cmake)

if (APPLE)
    set(PLUGIN_EXTENSION "dylib")
elseif (WIN32)
    set(PLUGIN_EXTENSION "dll")
else ()
    set(PLUGIN_EXTENSION "so")
endif ()

file(GLOB CODECS "${LIB_DIR}/Codec_*.${PLUGIN_EXTENSION}")
file(GLOB PLUGINS "${LIB_DIR}/Plugin_*.${PLUGIN_EXTENSION}")
file(GLOB RENDER_SYSTEMS "${LIB_DIR}/RenderSystem_*.${PLUGIN_EXTENSION}")

set(EXTRA_LIBS ${CODECS} ${PLUGINS} ${RENDER_SYSTEMS})
fixup_bundle("${EXECUTABLE}" "${EXTRA_LIBS}" "${CONAN_BIN_DIRS}")