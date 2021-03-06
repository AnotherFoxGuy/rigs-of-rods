find_package(Threads REQUIRED)

# some obsolete options:
# disabled some options for now
#set(ROR_FEAT_TIMING     "FALSE" CACHE BOOL "enable beam statistics. For core development only")
set(ROR_BUILD_UPDATER OFF)
set(ROR_FEAT_TIMING OFF)
set(ROR_BUILD_SIM ON)

if (USE_PACKAGE_MANAGER)
    set(ROR_USE_OPENAL TRUE)
    set(ROR_USE_SOCKETW TRUE)
    set(ROR_USE_PAGED TRUE)
    set(ROR_USE_CAELUM TRUE)
    set(ROR_USE_ANGELSCRIPT TRUE)
    set(ROR_USE_CURL TRUE)

    include(pmm)
    pmm(CONAN REMOTES rigs-of-rods-deps https://conan.cloudsmith.io/rigs-of-rods/deps/ BINCRAFTERS)

else (USE_PACKAGE_MANAGER)
    # components
    set(ROR_USE_OPENAL "TRUE" CACHE BOOL "use OPENAL")
    set(ROR_USE_SOCKETW "TRUE" CACHE BOOL "use SOCKETW")
    set(ROR_USE_PAGED "TRUE" CACHE BOOL "use paged-geometry")
    set(ROR_USE_CAELUM "TRUE" CACHE BOOL "use caelum sky")
    set(ROR_USE_ANGELSCRIPT "TRUE" CACHE BOOL "use angelscript")
    set(ROR_USE_CURL "TRUE" CACHE BOOL "use curl, required for communication with online services")

    # find packages
    find_package(OGRE 1.11 REQUIRED COMPONENTS Bites Overlay Paging RTShaderSystem MeshLodGenerator Terrain)
    find_package(OpenAL)
    find_package(OIS REQUIRED)
    find_package(MyGUI REQUIRED)
    find_package(SocketW)
    find_package(AngelScript)
    find_package(CURL)
    find_package(Caelum)
    find_package(fmt REQUIRED)

endif (USE_PACKAGE_MANAGER)
