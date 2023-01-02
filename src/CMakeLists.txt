add_library(entity_lib entity.c)

target_include_directories(entity_lib PUBLIC "../include")
target_compile_features(entity_lib PUBLIC c_std_99)

add_executable(publisher
  publisher.c)

idlc_generate(
  TARGET entity_idl_lib 
  FILES "../idl/entity_interface.idl"
  WARNINGS no-implicit-extensibility)

target_link_libraries(publisher 
  PRIVATE CycloneDDS::ddsc entity_idl_lib entity_lib)