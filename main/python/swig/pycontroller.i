%module PyController

// headers for generated file
%{
  #include "sim/python/PyController.h"

  using namespace stride;
  using namespace stride::python;
%}

// So we can pass python strings to arguments that require std::string.
%include std_string.i

%include "sim/python/PyController.h"
