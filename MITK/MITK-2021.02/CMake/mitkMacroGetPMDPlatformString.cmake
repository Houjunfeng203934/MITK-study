macro(mitkMacroGetPMDPlatformString varname)
  if(WIN32)
    if(CMAKE_CL_64)
      set(${varname} "W64")
    else()
      set(${varname} "W32")
    endif()
  else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
      set(${varname} "L64")
    else()
      set(${varname} "L32")
    endif()
  endif()
endmacro()