
#Estagio 1
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)    #define a versão apropriada para o CMake, 3.3. Geant4Config.cmake suporta a serie 2.6 e 2.8
project(Teste)   #nome do projeto

# NOVO
#preparing ROOT 
list(APPEND $ROOTSYS $ENV{ROOTSYS})



#Estagio 2
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)  #procura e configura o Geant4
else()
  find_package(Geant4 REQUIRED)
endif()

# NOVO
find_package(ROOT REQUIRED COMPONENTS RIO)

#Estagio 3: configura o uso do Geant4 e os Headers do projeto
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# NOVO
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})


#Estagio 4: lista os sources para construir a aplicação
#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#Estagio 5
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Teste Teste.cc ${sources} ${headers})  #define a construção da aplicação
target_link_libraries(Teste ${Geant4_LIBRARIES})

# NOVO
target_link_libraries(Teste ${ROOT_LIBRARIES})

#Estagio 6: copia qualquer scripts gerados para o diretório Build
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(TESTE_SCRIPTS
#  Teste.in
#  Teste.out
  init_vis.mac
#  run1.mac
#  run2.mac
  vis.mac
  )

foreach(_script ${TESTE_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(B1 DEPENDS exampleB1) opcional
#Estagio 7: instala o executável
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Teste DESTINATION bin)


