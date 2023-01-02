enable_testing()

add_executable(check_dds check_main.c)
target_compile_features(check_dds PUBLIC c_std_99)

target_link_libraries(check_dds PRIVATE Check::check Check::checkShared entity_lib)

add_test(
  NAME check_dds
  COMMAND check_dds)