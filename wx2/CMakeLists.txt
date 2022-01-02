add_library("${PROJECT_NAME}" "")
add_library(WX2::WX2 ALIAS "${PROJECT_NAME}")

set(WX2_EXPORT_FILE "wx2-exports")
install(TARGETS "${PROJECT_NAME}" EXPORT "${WX2_EXPORT_FILE}")

target_include_directories("${PROJECT_NAME}" PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" #WX2_export.h
	"$<INSTALL_INTERFACE:include>" #${CMAKE_INSTALL_PREFIX}/include
)

if(NOT BUILD_SHARED_LIBS)
	set(STATIC_POSTFIX "-s")
	set_target_properties("${PROJECT_NAME}" PROPERTIES 
	COMPILE_FLAGS -DWX2_STATIC_DEFINE)
endif()

#Rename target WX2
set_target_properties("${PROJECT_NAME}" PROPERTIES
	DEBUG_POSTFIX "${STATIC_POSTFIX}-d" 
	RELEASE_POSTFIX "${STATIC_POSTFIX}"
	MINSIZEREL_POSTFIX "${STATIC_POSTFIX}-mr"
	RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}-rd"
)

include("GenerateExportHeader")
generate_export_header("${PROJECT_NAME}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wx2_export.h" DESTINATION "include")


add_subdirectory("include")
add_subdirectory("src")

set(WX2_CONFIG_DIR "lib/cmake/${PROJECT_NAME}")
configure_file("${WX2_SOURCE_DIR}/cmake/wx2config.cmake.in" "${WX2_BINARY_DIR}/${WX2_CONFIG_DIR}/wx2config.cmake")
install(FILES "${WX2_BINARY_DIR}/${WX2_CONFIG_DIR}/wx2config.cmake" DESTINATION "${WX2_CONFIG_DIR}")
install(EXPORT "${WX2_EXPORT_FILE}" DESTINATION "${WX2_CONFIG_DIR}" NAMESPACE "WX2::")