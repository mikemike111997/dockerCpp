file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
${PROJECT_SOURCE_DIR}/*.[ch]pp
${PROJECT_SOURCE_DIR}/*.h
)

find_program(cppcheck "cppcheck")
if(cppcheck)
    add_custom_target(
            run-cppcheck
            COMMAND ${cppcheck}
             --enable=warning,performance,portability,information,missingInclude
            --enable=all
            --std=c++17
            --language=c++
            -I ${PROJECT_SOURCE_DIR}/inc
            --suppress=missingIncludeSystem
            --verbose
            ${ALL_CXX_SOURCE_FILES}
    )
else()
    message("cppcheck was not found.")
endif()