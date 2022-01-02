%module adtree

%{
#define SWIG_FILE_WITH_INIT
#include "adtree.h"
%}

%include "numpy.i"

%init %{
import_array();
%}

%apply (int* IN_ARRAY1, int DIM1) {(int* data, int datasize)}
%apply (int* IN_ARRAY1, int DIM1) {(int* variables, int nvariables)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int* contab, int contabsize)}
%include "adtree.h"
