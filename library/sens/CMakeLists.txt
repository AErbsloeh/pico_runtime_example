file(GLOB CHILD_FOLDERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(folder ${CHILD_FOLDERS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${folder})
        add_subdirectory(${folder})
    endif()
endforeach()
