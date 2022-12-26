cmake_minimum_required(VERSION 3.23)
project(dds-testing LANGUAGES C)

find_package(CycloneDDS REQUIRED)

add_executable(publisher src/publisher.c)

idlc_generate(TARGET vehicle_lib FILES idl/vehicle.idl WARNINGS no-implicit-extensibility)

target_link_libraries(publisher PRIVATE CycloneDDS::ddsc vehicle_lib)

install(
  TARGETS publisher
  EXPORT "${PROJECT_NAME}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT publisher
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT publisher
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT publisher)