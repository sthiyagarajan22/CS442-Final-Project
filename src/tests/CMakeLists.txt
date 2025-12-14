## Grab all .cpp files in test directory
file(GLOB CPP_SOURCES CONFIGURE_DEPENDS "*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/src/refs)

# Import the precompiled library
add_library(refs STATIC IMPORTED)
set_target_properties(refs PROPERTIES
	IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/src/refs/testReference.a"
)

## Go through each CPP file
foreach(src ${CPP_SOURCES})
    
    ## Grab the name without full path or extension
    get_filename_component(exec_name ${src} NAME_WE)

    ## Create executable
    add_executable(${exec_name} ${src})

    # Link with MPI 
    target_link_libraries(${exec_name} tutorial ${MPI_LIBRARIES} refs)

    if(exec_name STREQUAL "test_read" OR exec_name STREQUAL "test_write")
	continue()
    endif()

    # Add to CTEST
    add_test(NAME ${exec_name}_Test COMMAND ${MPIRUN} -n 2 ./${exec_name})

endforeach()

add_custom_target(generateMatrix
    COMMAND $<TARGET_FILE:matrixBuilder> 64
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating matrix.bin for tests"
)

add_dependencies(test_read generateMatrix)
add_dependencies(test_write generateMatrix)

add_test(NAME test_read_Test
    COMMAND ${MPIRUN} -n 2 ${CMAKE_CURRENT_BINARY_DIR}/test_read matrix.bin
)


add_test(NAME test_write_Test
    COMMAND ${MPIRUN} -n 2 ${CMAKE_CURRENT_BINARY_DIR}/test_write
)

set_tests_properties(test_init_Test test_finalize_Test test_read_Test PROPERTIES LABELS "read;write")

set_tests_properties(test_write_Test PROPERTIES LABELS "write")


