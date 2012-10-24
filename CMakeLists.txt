##############################################################################
# include the header files
##############################################################################

#one below that UtilLib is included in the paths they are needed for the cmake config files
INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/../
   ${CMAKE_CURRENT_BINARY_DIR}/../
)

##############################################################################
# add the source files
##############################################################################
set(UTILLIB_SRCS


    src/Exception.cpp
    src/ProgressBar.cpp
    src/MPIProxy.cpp
    src/Log.cpp

)

set(UTILLIB_HEADER

    include/Exception.hpp
    include/Factory.hpp
    include/ProgressBar.hpp
    include/MPIProxy.hpp
    include/Log.hpp
    include/Singleton.hpp

)

#############################################################################
# Libraries
#############################################################################
if (UNIX)
   ADD_DEFINITIONS("-fno-strict-aliasing" )
endif (UNIX)
set ( LIBLIST
    ${MPI_LIBRARIES}
    ${Boost_LIBRARIES}
)

add_library( ${LBMSOLVER_LIBRARY_PREFIX}UtilLib ${LIB_TYPE} ${UTILLIB_SRCS} ${UTILLIB_HEADER})

target_link_libraries( ${LBMSOLVER_LIBRARY_PREFIX}UtilLib ${LIBLIST} )

#############################################################################
# Cmake generated header files
#############################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
)


#############################################################################
# testing
#############################################################################

IF(BUILD_TESTING)
        ADD_SUBDIRECTORY(test)
ENDIF(BUILD_TESTING)



install( TARGETS ${LBMSOLVER_LIBRARY_PREFIX}UtilLib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )
