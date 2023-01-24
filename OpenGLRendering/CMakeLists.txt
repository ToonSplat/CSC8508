set(PROJECT_NAME OpenGLRendering)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "OGLComputeShader.h"
    "OGLMesh.h"
    "OGLRenderer.h"
    "OGLShader.h"
    "OGLTexture.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "OGLComputeShader.cpp"
    "OGLMesh.cpp"
    "OGLRenderer.cpp"
    "OGLShader.cpp"
    "OGLTexture.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(GLAD
    "glad/gl.h"
    "glad/gl.c"
)
source_group("GLAD" FILES ${GLAD})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${GLAD}
)

set_source_files_properties(${ALL_FILES} PROPERTIES LANGUAGE CXX)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE OpenGLRendering)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ";"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE
#        $<$<CONFIG:Release>:
#            /Oi;
##            /Gy
 #       >
#        /permissive-;
#        /std:c++latest;
#        /sdl;
 #       /W3;
  #      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
   #     ${DEFAULT_CXX_EXCEPTION_HANDLING};
    #    /Y-
   # )
   # target_link_options(${PROJECT_NAME} PRIVATE
   #     $<$<CONFIG:Release>:
   #         /OPT:REF;
   #         /OPT:ICF
   #     >
   # )
#endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "NCLCoreClasses"
    "opengl32.lib"
)
include_directories("../NCLCoreClasses/")
include_directories("../OpenGLRendering/")

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
