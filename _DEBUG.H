// ==========================================================================
// _DEBUG macros library ver. 1.0 (C, CPP)
// Troy computing, Inc.
// Copyright (c) 1994-'98
// --------------------------------------------------------------------------
// Macros library
// Used library: stdio.h
// --------------------------------------------------------------------------
// File: _debug.h
// ==========================================================================
// Made date: 05/XII.1998
// Last edit: 05/XII.1998
// Build: 1
// ==========================================================================
// Constants for debug
// --------------------------------------------------------------------------
// #include 
// --------------------------------------------------------------------------
// Names of output files (changeable)
// --------------------------------------------------------------------------
#define _DEBUG_FILETXT "_STACK80211.log"
#define _DEBUG_FILEBIN "_STACK80211.dat"
// --------------------------------------------------------------------------
// Make page break after given number of variables (20.01.2003)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEVECT4(_flnm_, _vector_, _size_, _format_, _cnv_, _num_) \
{ int i; FILE* _d_fp=fopen(_flnm_, "wt");\
for (i=0; i<_size_; i++)\
fprintf(_d_fp, (i%_num_)?_format_", ":"\n\n"_format_", ",\
(_cnv_)_vector_[i]);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// Trace stream or variable (17.01.2003)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEVAR(_flnm_, _frmt_, _var_) \
{ FILE* _d_fp=fopen(_flnm_, "at");\
fprintf(_d_fp, _frmt_, _var_);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// Saves vector's elements into a text file with given filename using given conversion (03.12.2002)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEVECT3(_flnm_, _vector_, _size_, _format_, _cnv_) \
{ int i; FILE* _d_fp=fopen(_flnm_, "wt");\
for (i=0; i<_size_; i++)\
fprintf(_d_fp, _format_", ",(_cnv_)_vector_[i]);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// Saves vector's elements into a text file with given filename (11.11.2002)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEVECT2(_flnm_, _vector_, _size_, _format_) \
{ int i; FILE* _d_fp=fopen(_flnm_, "wt");\
for (i=0; i<_size_; i++)\
fprintf(_d_fp, _format_", ",_vector_[i]);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// Saves vector's elements into a text file (11.11.2002)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEVECT(_vector_, _size_, _format_) \
{ int i; FILE* _d_fp=fopen(#_vector_".txt", "wt");\
for (i=0; i<_size_; i++)\
fprintf(_d_fp, _format_", ",_vector_[i]);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// String tracing (do not change)
// --------------------------------------------------------------------------
#define _DEBUG_TRACES(_text_, _value_) \
{ FILE* _d_fp=fopen(_DEBUG_FILETXT, "at");\
fprintf(_d_fp, #_value_" ("_text_"):\n---\n"\
"%s\n---\n",_value_);\
fclose(_d_fp);\
}
// --------------------------------------------------------------------------
// Tracing of a binary array (do not change)
// --------------------------------------------------------------------------
#define _DEBUG_TRACEB(_value_, _size_) \
{\
FILE* _d_fp=fopen(_DEBUG_FILEBIN, "wb");\
fwrite(_value_, 1, _size_, _d_fp);\
fclose(_d_fp);\
}
// ==========================================================================
