cmake_minimum_required(VERSION 3.9)
project(demo)

add_executable(demo)
target_sources(demo PRIVATE
	"Demo/src/main.cpp"
	"Demo/include/test_layer.hpp"
	"Demo/src/test_layer.cpp"
	"Demo/src/utils_layer.cpp" "Demo/src/rat_layer.cpp")

target_include_directories(demo PRIVATE "Demo/include")
target_compile_features(demo PRIVATE cxx_std_17)

add_subdirectory("Akila")
include_directories("Akila/include")
target_link_libraries(demo akila)

if(WIN32)
	set_target_properties(${PROJECT_NAME}
		PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
		LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

#[[
#Copie les ressources
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/Demo/resources
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
#]]

##[[
#Creer un symlink, beaucoup mieux, mais cmake doit avoir les droits
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
		${PROJECT_SOURCE_DIR}/Demo/resources
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
#]]
