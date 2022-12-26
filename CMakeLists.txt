cmake_minimum_required(VERSION 3.23)
project(dds-testing VERSION 0.0.1 LANGUAGES C)

find_package(CycloneDDS REQUIRED)

idlc_generate(TARGET vehicle_lib FILES idl/vehicle.idl)