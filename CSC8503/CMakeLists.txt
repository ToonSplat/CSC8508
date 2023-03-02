set(PROJECT_NAME CSC8503)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "ApplyPaint.h"
    "PaintableObject.h"
    "Team.h"
    "PaintBallClass.h"
    "PaintBallProjectile.h"
    "Player.h"
    "ImpactPoint.h"
    "ToonLevelManager.h"
    "ToonGame.h"
    "HitSphere.h"
    "ToonGameObject.h"
    "ToonEventListener.h"
    "ToonTransform.h"
    "ToonGameWorld.h"
    "ToonRaycastCallback.h"
    "ToonUtils.h"
    "PlayerControl.h"
    "ToonMapCamera.h"
    "ToonMinimapCamera.h"
    "ToonGameObjectAnim.h"
    "ToonDebugManager.h"
    "ToonConfirmationScreen.h"
    "Toon2DCoordinates.h"
    "ToonGameSettings.h"
    "ToonToggleButton.h"
    "ToonFileHandling.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "ApplyPaint.cpp"
    "Main.cpp"
    "PaintableObject.cpp"
    "Team.cpp"
    "PaintBallClass.cpp"
    "PaintBallProjectile.cpp"
    "Player.cpp"
    "ToonLevelManager.cpp"
    "ToonGame.cpp"
    "HitSphere.cpp"
    "ToonGameObject.cpp"
    "ToonEventListener.cpp"
    "ToonTransform.cpp"
    "ToonGameWorld.cpp"
    "ToonGameObjectAnim.cpp"
    "ToonDebugManager.cpp"
    "ToonConfirmationScreen.cpp"
    "ToonGameSettings.cpp"
    "ToonToggleButton.cpp"
    "ToonFileHandling.cpp"
)
source_group("Source Files" FILES ${Source_Files})
set(Networking_Files
    "ToonNetworkedGame.h"
    "ToonNetworkedGame.cpp"
    "ToonNetworkObject.h"
    "ToonNetworkObject.cpp"
    "ToonObjectState.h"
    "ToonNetworkPackets.h"
)
source_group("Networking Files" FILES ${Networking_Files})

set(Rendering_Files
    "ToonMeshMaterial.h"
    "ToonMeshMaterial.cpp"
    "ToonAssetManager.h"
    "ToonAssetManager.cpp"
    "GameTechRenderer.h"
    "GameTechRenderer.cpp"
    "ToonRenderObject.h"
    "ToonRenderObject.cpp"
)
source_group("Rendering Files" FILES ${Rendering_Files})

set(Inputs_Files
    "PlayerControl.h"
    "PlayerControl.cpp"
    "BaseInput.h"
    "KeyboardInput.h"
    "KeyboardInput.cpp"
    "XboxControllerInput.h"
    "XboxControllerInput.cpp"
)
source_group("Inputs Files" FILES ${Inputs_Files})

set(Camera_Files
    "ToonFollowCamera.h"
    "ToonMinimapCamera.h"
    "ToonObserverCamera.h"
    "ToonMapCamera.h"
    "ToonFollowCamera.cpp"
    "ToonMinimapCamera.cpp"
    "ToonObserverCamera.cpp"
    "ToonMapCamera.cpp"
)
source_group("Camera Files" FILES ${Camera_Files})

set(Menu_Files
    "ToonMainMenu.h"
    "ToonTextInput.h"
    "ToonVirtualKeyboard.h"
    "ToonMainMenu.cpp"
    "ToonTextInput.cpp"
    "ToonVirtualKeyboard.cpp"
)
source_group("Menu Files" FILES ${Menu_Files})

if(USE_VULKAN)
list(APPEND Header_Files "GameTechVulkanRenderer.h")
list(APPEND Source_Files "GameTechVulkanRenderer.cpp")

file(GLOB SHADER_FILES ${ASSET_ROOT}/Shaders/VK/*.*)

endif()

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${Networking_Files}
    ${Rendering_Files}
    ${Inputs_Files}
    ${Camera_Files}
    ${Menu_Files}
)

if(USE_VULKAN)
    set(ALL_FILES 
        ${Header_Files}
        ${Source_Files}
        ${Networking_Files}
        ${Rendering_Files}
        ${Inputs_Files}
        ${Camera_Files}
        ${Menu_Files}
        ${SHADER_FILES}
    )

foreach (file ${SHADER_FILES})
    get_filename_component(file_name ${file} NAME)
	get_filename_component(file_ext ${file} EXT)
	
	if(file_ext STREQUAL ".h" OR file_ext STREQUAL ".cpp")
		continue()
	endif()
		
	if( file_ext STREQUAL  ".vert" OR
		file_ext STREQUAL  ".frag" OR
		file_ext STREQUAL  ".comp" OR
		file_ext STREQUAL  ".geom" OR
		file_ext STREQUAL  ".tesc" OR
		file_ext STREQUAL  ".tese" OR
		file_ext STREQUAL  ".rgen" OR
		file_ext STREQUAL  ".rint" OR
		file_ext STREQUAL  ".rahit" OR
		file_ext STREQUAL  ".rchit" OR
		file_ext STREQUAL  ".rmiss" OR
		file_ext STREQUAL  ".rcall" OR
		file_ext STREQUAL  ".task" OR
		file_ext STREQUAL  ".mesh"
	)
		message("Adding custom command to ${file}")
		get_filename_component(file_dir ${file} ABSOLUTE)
        set(SPIRV_OUTPUT ${file_name}.spv)
		set(SPIRV_ABS_INPUT ${file_dir})
		set(SPIRV_ABS_OUTPUT ${file_dir}.spv)
		
		add_custom_command(
			OUTPUT ${SPIRV_ABS_OUTPUT}
			
			COMMENT "Compiling GLSL shader:"
			COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V  ${SPIRV_ABS_INPUT} -o ${SPIRV_ABS_OUTPUT}
            DEPENDS ${file}
			VERBATIM
		)
        list(APPEND SPIRV_BINARY_FILES ${SPIRV_OUTPUT})   
	endif()
endforeach()

endif()

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME}  ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE CSC8503)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

################################################################################
# Compile definitions
################################################################################
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "UNICODE;"
        "_UNICODE"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
        "_WINSOCKAPI_"   
        "_WINSOCK2API_"
        "_CRT_SECURE_NO_WARNINGS;"
        "_WINSOCK_DEPRECATED_NO_WARNINGS"
    )
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <vector>
    <map>
    <stack>
    <string>
    <list>
    <thread>
    <atomic>
    <functional>
    <iostream>
    <set>
    "../NCLCoreClasses/Vector2.h"
    "../NCLCoreClasses/Vector3.h"
    "../NCLCoreClasses/Vector4.h"
    "../NCLCoreClasses/Quaternion.h"
    "../NCLCoreClasses/Plane.h"
    "../NCLCoreClasses/Matrix2.h"
    "../NCLCoreClasses/Matrix3.h"
    "../NCLCoreClasses/Matrix4.h"
    
)
 
if(USE_VULKAN)
target_precompile_headers(${PROJECT_NAME} PRIVATE
    "../VulkanRendering/SmartTypes.h"

    "../VulkanRendering/VulkanRenderer.h"
    "../VulkanRendering/VulkanShader.h"
    "../VulkanRendering/VulkanShaderBuilder.h"
    "../VulkanRendering/VulkanTexture.h"
    "../VulkanRendering/VulkanMesh.h"
    "../VulkanRendering/VulkanPipelineBuilder.h"
    "../VulkanRendering/VulkanDynamicRenderBuilder.h"

    "../VulkanRendering/VulkanDescriptorSetLayoutBuilder.h"
    "../VulkanRendering/VulkanRenderPassBuilder.h"

    "../VulkanRendering/VulkanCompute.h"
    "../VulkanRendering/VulkanComputePipelineBuilder.h"
)
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++latest;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

if(WIN32) 
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ReactPhysics3D") 
endif() 

#Import the ReactPhysics3D library 
find_package(ReactPhysics3D REQUIRED)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "ReactPhysics3D::ReactPhysics3D"
)

find_library(XINPUT_LIBRARY NAMES XInput)
if(NOT XINPUT_LIBRARY)
    message(FATAL_ERROR "XInput library not found")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${XINPUT_LIBRARY})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
################################################################################
# Dependencies
################################################################################
if(MSVC)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "Winmm.lib")
endif()

include_directories("../OpenGLRendering/")
include_directories("../NCLCoreClasses/")
include_directories("../CSC8503CoreClasses/")

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC NCLCoreClasses)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC CSC8503CoreClasses)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGLRendering)

if(USE_VULKAN)
include_directories("../VulkanRendering/")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC VulkanRendering)


add_custom_target(
    SHADER_FILES
    DEPENDS ON ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} SHADER_FILES)

endif()