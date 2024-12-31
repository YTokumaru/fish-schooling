function(get_binary_version output_var binary_path)
  if(EXISTS ${binary_path})
    execute_process(
      COMMAND ${binary_path} --version
      OUTPUT_VARIABLE output
      ERROR_QUIET
    )

    string(
      REGEX
      MATCH
      "[0-9]+\\.[0-9]+(\\.[0-9]+)?"
      version
      "${output}"
    )

    if(version)
      set(${output_var} ${version} PARENT_SCOPE)
    else()
      message(WARNING "Could not retrieve version from binary: ${binary_path}")
    endif()
  else()
    message(FATAL_ERROR "Binary not found: ${binary_path}")
  endif()
endfunction(get_binary_version)