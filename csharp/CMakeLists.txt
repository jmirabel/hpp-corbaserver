CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(hpp-corbaserver LANGUAGES CSharp)

SET(VISUAL_STUDIO_DOTNET_FRAMEWORK_VERSION "v4.6.1" CACHE STRING "Set the version of the .NET framework")

# Generate CLS from IDL
FIND_PROGRAM(IIOP_IDL "IDL TO CLS Compiler CS.exe")
FIND_FILE(IIOPCHANNEL_LIB IIOPChannel.dll
	PATH_SUFFIXES bin lib)
FIND_FILE(IIOPCHANNEL_nunit_LIB nunit.framework.dll
	PATH_SUFFIXES bin lib)

IF(NOT IIOP_IDL)
	MESSAGE(FATAL_ERROR "\"IDL TO CLS Compiler CS.exe\" not found.")
ENDIF()
IF(NOT IIOPCHANNEL_LIB)
	MESSAGE(FATAL_ERROR "Could not find IIOPChannel")
ENDIF()

SET(IDL_DIR "${CMAKE_SOURCE_DIR}/../idl")
SET(IDL_FILES
	"${IDL_DIR}/hpp/corbaserver/common.idl"
	"${IDL_DIR}/hpp/corbaserver/robot.idl"
	"${IDL_DIR}/hpp/corbaserver/problem.idl"
	"${IDL_DIR}/hpp/corbaserver/obstacle.idl"
	)

ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_BINARY_DIR}/hpp-corbaserver-idl.dll
	COMMAND "${IIOP_IDL}"
	ARGS -idir "${IDL_DIR}/../idl"
	-o ${CMAKE_BINARY_DIR} hpp-corbaserver-idl
	${IDL_FILES})

ADD_CUSTOM_TARGET(generate_idl_csharp DEPENDS ${CMAKE_BINARY_DIR}/hpp-corbaserver-idl.dll)

ADD_LIBRARY(hpp-corbaserver SHARED src/client.cs)
SET_TARGET_PROPERTIES(hpp-corbaserver
	PROPERTIES
	VS_DOTNET_REFERENCE_IIOP_Channel "${IIOPCHANNEL_LIB}"
	VS_DOTNET_REFERENCE_IIOP_Channel_nunit "${IIOPCHANNEL_nunit_LIB}"
	VS_DOTNET_REFERENCE_HPP_CORBASERVER_IDL "${CMAKE_BINARY_DIR}/hpp-corbaserver-idl.dll"
	VS_DOTNET_TARGET_FRAMEWORK_VERSION ${VISUAL_STUDIO_DOTNET_FRAMEWORK_VERSION}
	)
ADD_DEPENDENCIES(hpp-corbaserver generate_idl_csharp)

ADD_EXECUTABLE(check-connection src/check-connection.cs)
SET_TARGET_PROPERTIES(check-connection
	PROPERTIES
	VS_DOTNET_REFERENCE_IIOP_Channel "${IIOPCHANNEL_LIB}"
	VS_DOTNET_REFERENCE_IIOP_Channel_nunit "${IIOPCHANNEL_nunit_LIB}"
	VS_DOTNET_REFERENCE_HPP_CORBASERVER_IDL "${CMAKE_BINARY_DIR}/hpp-corbaserver-idl.dll"
	VS_DOTNET_TARGET_FRAMEWORK_VERSION ${VISUAL_STUDIO_DOTNET_FRAMEWORK_VERSION}
	)
TARGET_LINK_LIBRARIES(check-connection hpp-corbaserver)

INSTALL(TARGETS hpp-corbaserver check-connection DESTINATION bin)
INSTALL(FILES "${CMAKE_BINARY_DIR}/hpp-corbaserver-idl.dll" DESTINATION bin)
INSTALL(FILES ${IDL_FILES} DESTINATION share/idl/hpp/corbaserver)
