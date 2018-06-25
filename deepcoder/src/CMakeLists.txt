add_executable(gen_dataset generate_dataset.cc)
target_link_libraries(gen_dataset deepcoder)
target_link_libraries(gen_dataset pthread)

add_executable(gen_program generate_program.cc)
target_link_libraries(gen_program deepcoder)
target_link_libraries(gen_program pthread)

add_executable(gen_testdata generate_testdata.cc)
target_link_libraries(gen_testdata deepcoder)
target_link_libraries(gen_testdata pthread)

add_executable(eval_program evaluate_program.cc)
target_link_libraries(eval_program deepcoder)
target_link_libraries(eval_program pthread)

add_executable(DM_cmd DM_cmd.cc)
target_link_libraries(DM_cmd  deepcoder)
target_link_libraries(DM_cmd pthread)

add_executable(test_DM test_DM.cc)
target_link_libraries(test_DM deepcoder)
target_link_libraries(test_DM pthread)

