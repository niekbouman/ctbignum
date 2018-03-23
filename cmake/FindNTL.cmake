# Try to find the NTL (Victor Shoup's) number theory library

if (NTL_INCLUDES AND NTL_LIBRARIES)
  set(NTL_FIND_QUIETLY TRUE)
endif (NTL_INCLUDES AND NTL_LIBRARIES)

find_path(NTL_INCLUDES
  NAMES
    NTL/ZZ.h
  PATHS
    $ENV{NTLDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(NTL_LIBRARIES ntl PATHS $ENV{NTLDIR} ${LIB_INSTALL_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NTL DEFAULT_MSG
                                  NTL_INCLUDES NTL_LIBRARIES)
mark_as_advanced(NTL_INCLUDES NTL_LIBRARIES)
