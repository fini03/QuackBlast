file(GLOB MAIN_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/examples/quackblast/eventlisteners/*.cpp
    ${CMAKE_SOURCE_DIR}/examples/quackblast/astar/AStar.cpp
    ${CMAKE_SOURCE_DIR}/examples/quackblast/game/MyVulkanEngine.cpp
)

add_executable( main 
    main.cpp 
    ${MAIN_SOURCE_FILES}
    ${CMAKE_SOURCE_DIR}/include/VPE.hpp 
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Release
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Debug
)

# Link the VVE
set( VulkanEngine ${CMAKE_SOURCE_DIR}/../../ViennaVulkanEngine )
include_directories(${VulkanEngine}/VulkanEngine)

target_include_directories(main PUBLIC
          ${CMAKE_SOURCE_DIR}/include
          ${VulkanEngine}/VulkanEngine
          ${VulkanEngine}/external/threadpool
          ${VulkanEngine}/external/stb
          ${VulkanEngine}/external/nuklear
          )

set(IRRKLANG_ROOT ${VulkanEngine}/external/irrKlang)
find_package(IrrKlang REQUIRED)
target_include_directories(main PUBLIC ${IRRKLANG_INCLUDE_DIR})
target_link_libraries(main ${IRRKLANG_LIBRARY})

find_package(ZLIB REQUIRED)
target_link_libraries(main ZLIB::ZLIB)

find_package(assimp REQUIRED)
target_include_directories(main PUBLIC ${assimp_INCLUDE_DIRS})
target_link_libraries(main assimp::assimp)

find_package(glm CONFIG REQUIRED)
target_include_directories(main PUBLIC ${glm_INCLUDE_DIRS})
target_link_libraries(main glm)

find_package(glfw3 CONFIG REQUIRED)
target_include_directories(main PUBLIC ${glfw_INCLUDE_DIRS})
target_link_libraries(main glfw)

add_library(VulkanEngineLib STATIC IMPORTED)
set_target_properties(VulkanEngineLib PROPERTIES
             IMPORTED_CONFIGURATIONS "Debug;Release"
             IMPORTED_LOCATION ${VulkanEngine}/lib/libvulkanengine.a
             IMPORTED_LOCATION_DEBUG ${VulkanEngine}/lib/libvulkanengine.a
             )

target_link_libraries(main VulkanEngineLib)
