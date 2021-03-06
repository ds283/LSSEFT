CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

INCLUDE(ExternalProject)


FUNCTION(BUILD_CUBA BUILD_DIR INSTALL_PREFIX)

  SET(CUBA_SRC ${BUILD_DIR}/src)
  SET(CUBA_BIN ${BUILD_DIR}/bin)
  SET(CUBA_INSTALL ${INSTALL_PREFIX})
  SET(CUBA_INSTALL ${INSTALL_PREFIX} PARENT_SCOPE)

  EXTERNALPROJECT_ADD(
    CUBA
    URL http://www.feynarts.de/cuba/Cuba-4.2.tar.gz
    URL_MD5 f9ef09cb7d1771f46dd21cd8b8346356
    SOURCE_DIR ${CUBA_SRC}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${CUBA_SRC}/configure --prefix=${INSTALL_PREFIX}
    BUILD_COMMAND make lib
  )

  SET(CUBA_LIBRARIES ${CUBA_INSTALL}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}cuba${CMAKE_STATIC_LIBRARY_SUFFIX} PARENT_SCOPE)
  SET(CUBA_INCLUDE_DIRS ${CUBA_INSTALL}/include PARENT_SCOPE)

  ADD_DEPENDENCIES(DEPS CUBA)

ENDFUNCTION(BUILD_CUBA)
