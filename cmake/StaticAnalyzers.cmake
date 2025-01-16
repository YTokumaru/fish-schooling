include(cmake/Utility.cmake)

macro(project_enable_cppcheck TARGET WARNINGS_AS_ERRORS)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    get_binary_version(CPPCHECK_VERSION ${CPPCHECK})
    if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
      set(CPPCHECK_TEMPLATE "vs")
    else()
      set(CPPCHECK_TEMPLATE "gcc")
    endif()

    # Enable all warnings that are actionable by the user of this toolset
    # style should enable the other 3, but we'll be explicit just in case
    set(OPTION_CXX_CPPCHECK
        ${CPPCHECK}
        --template=${CPPCHECK_TEMPLATE}
        --enable=style,performance,warning,portability
        --inline-suppr
        # We cannot act on a bug/missing feature of cppcheck
        --suppress=cppcheckError
        --suppress=internalAstError
        # if a file does not have an internalAstError, we get an unmatchedSuppression error
        --suppress=unmatchedSuppression
        # noisy and incorrect sometimes
        --suppress=passedByValue
        # ignores code that cppcheck thinks is invalid C++
        --suppress=syntaxError
        --suppress=preprocessorErrorDirective
        --inconclusive)

    if(NOT
       "${CMAKE_CXX_STANDARD}"
       STREQUAL
       "")
      set(OPTION_CXX_CPPCHECK ${OPTION_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
    endif()
    if(${WARNINGS_AS_ERRORS})
      list(APPEND OPTION_CXX_CPPCHECK --error-exitcode=2)
    endif()
    message(STATUS "${CPPCHECK} ${CPPCHECK_VERSION}: found")
    message(STATUS "Enabling cppcheck with options: ${OPTION_CXX_CPPCHECK}")
    set_target_properties(${TARGET} PROPERTIES CXX_CPPCHECK "${OPTION_CXX_CPPCHECK}")
  else()
    message(WARNING "cppcheck requested but executable not found")
  endif()
endmacro()

macro(project_enable_clang_tidy TARGET WARNINGS_AS_ERRORS)

  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    get_binary_version(CLANGTIDY_VERSION ${CLANGTIDY})

    # construct the clang-tidy command line
    set(CLANG_TIDY_OPTIONS
        ${CLANGTIDY}
        -extra-arg=-Wno-unknown-warning-option
        -extra-arg=-Wno-ignored-optimization-argument
        -extra-arg=-Wno-unused-command-line-argument
        -p)
    # set standard
    if(NOT
       "${CMAKE_CXX_STANDARD}"
       STREQUAL
       "")
      if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
      else()
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
      endif()
    endif()

    # set warnings as errors
    if(${WARNINGS_AS_ERRORS})
      list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
    endif()
    message(STATUS "${CLANGTIDY} ${CLANGTIDY_VERSION}: found")
    set_property(TARGET ${TARGET} APPEND PROPERTY CXX_CLANG_TIDY "${CLANG_TIDY_OPTIONS}")
  else()
    message(WARNING "clang-tidy requested but executable not found")
  endif()
endmacro()

macro(myproject_enable_include_what_you_use)
  find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
  if(INCLUDE_WHAT_YOU_USE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
  else()
    message(${WARNING_MESSAGE} "include-what-you-use requested but executable not found")
  endif()
endmacro()