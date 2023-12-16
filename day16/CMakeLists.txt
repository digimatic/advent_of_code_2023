get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(${PARALLEL_ENABLE})
	find_package(TBB REQUIRED)
endif()

configure_file(config.h.in config.h)

add_executable(${PROJECT_NAME} "main.cpp" input.txt inputExample1.txt config.h.in)

if(${PARALLEL_ENABLE})
	target_compile_definitions(${PROJECT_NAME} PUBLIC PARALLEL)
	target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
