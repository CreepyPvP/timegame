cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(timegame)

# Clang deez nuts
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include debug information
set(CMAKE_BUILD_TYPE Debug)


# Add all Header Files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)


IF (WIN32)
    # Windows libraries:
    find_package(OpenGL REQUIRED)
    set(LIBS glfw opengl32 glad glm)

    file(GLOB_RECURSE SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/platform/windows.cpp
    )
ELSE()
    # Linux libraries:
    set(LIBS glfw X11 glad glm)

    file(GLOB_RECURSE SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/platform/linux.cpp
    )
ENDIF()

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Variables for Paths of External Libraries
set(GLFW_ROOT_DIR external/glfw)
set(GLAD_ROOT_DIR external/glad)
set(GLM_ROOT_DIR external/glm)


# Add the External Libraries / Files Directory
add_subdirectory(${GLFW_ROOT_DIR})
add_subdirectory(${GLAD_ROOT_DIR})
add_subdirectory(${GLM_ROOT_DIR})

# Add the Include Directories for the Libraries / Files
#Used for adding #include into the cpp files

#		Include Files From Within the Workspace Folder

target_include_directories(${PROJECT_NAME} 
	PUBLIC ${GLFW_ROOT_DIR}/include
    PUBLIC ${GLAD_ROOT_DIR}/include/glad
    PUBLIC ${GLM_ROOT_DIR}/include
    PUBLIC include
)

#		Include Files from Outside the Workspace Folder -> Like Vulkan SDK

#target_include_directories(${PROJECT_NAME}
#	PRIVATE ${Vulkan_INCLUDE_DIRS}
#)

# Add the Link Directories for the Libraries / Files
#Used for creation of Static/Dynamic Libraries

#		Include Files From Within the Workspace Folder

target_link_directories(${PROJECT_NAME} 
	PRIVATE ${GLFW_ROOT_DIR}/src
	PRIVATE ${GLAD_ROOT_DIR}/src
    PRIVATE ${GLM_ROOT_DIR}/src
)

# Add the Libs to the Linker for the Project

target_link_libraries(${PROJECT_NAME} ${LIBS})

# EXPLANATION FOR SOME STUFF

# GLOB_RECURSE -> Recursively go through all subdirectories
#		of the given path and get all the files matching the extension provided
# file() -> Store the Files matching the RegEx into a variable
# set() -> Alternative to file() if you want to set variables for a custom behaviour
