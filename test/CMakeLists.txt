add_executable(STL_Test "main_test.cpp")
include_directories(
        ./packages/Corsac_Test/include
)
target_link_libraries(
        STL_Test PRIVATE
        corsac::STL
)