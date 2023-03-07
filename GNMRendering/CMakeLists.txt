set(PROJECT_NAME GNMRendering)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "PS4ComputeShader.h"
    "PS4Mesh.h"
 
    "PS4Shader.h"
    "PS4Texture.h"
"PS4AudioEmitter.h"
"PS4AudioSource.h"
"PS4Frame.h"
"PS4Input.h"
"PS4MemoryAware.h"
"PS4RendererBase.h"
"PS4Sound.h"
"PS4Window.h"

)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "PS4ComputeShader.cpp"
    "PS4Mesh.cpp"
    
    "PS4Shader.cpp"
    "PS4Texture.cpp"
"PS4AudioEmitter.cpp"
"PS4AudioSource.cpp"
"PS4Frame.cpp"
"PS4Input.cpp"
"PS4MemoryAware.cpp"
"PS4RendererBase.cpp"
"PS4Sound.cpp"
"PS4Window.cpp"

)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    
)

set_source_files_properties(${ALL_FILES} PROPERTIES LANGUAGE CXX)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GNMRendering)

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
   
)
include_directories("../NCLCoreClasses/")
include_directories("../GNMRendering/")

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
