#ifndef __FASTJET_ERROR_HH__
#define __FASTJET_ERROR_HH__

//FJSTARTHEADER
// $Id: Error.hh 3694 2014-09-18 13:21:54Z soyez $
//
// Copyright (c) 2005-2014, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development. They are described in the original FastJet paper,
//  hep-ph/0512210 and in the manual, arXiv:1111.6097. If you use
//  FastJet as part of work towards a scientific publication, please
//  quote the version you use and include a citation to the manual and
//  optionally also to hep-ph/0512210.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//FJENDHEADER

#include<iostream>
#include<string>
#include "fastjet/internal/base.hh"
#include "fastjet/config.h"
#if (!defined(FASTJET_HAVE_EXECINFO_H)) || defined(__FJCORE__)
#include "fastjet/LimitedWarning.hh"
#endif

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// @ingroup error_handling
/// \class Error
/// base class corresponding to errors that can be thrown by FastJet
class Error {
public:
  /// default constructors
  Error() {}

  /// ctor from an error message
  ///   \param message to be printed
  /// Note: in addition to the error message, one can choose to print the
  /// backtrace (showing the last few calls before the error) by 
  /// using set_print_backtrace(true). The default is "false".
  Error(const std::string & message);

  /// virtual dummy dtor
  virtual ~Error() {}

  /// the error message
  std::string message() const {return _message;}


  // CMS change: the following three static functions 
  //  are no longer defined in the header.
  // Change not endorsed by fastjet collaboration 

  /// controls whether the error message (and the backtrace, if its printing is enabled) 
  /// is printed out or not
  static void set_print_errors(bool print_errors);// {_print_errors = print_errors;}

  /// controls whether the backtrace is printed out with the error message or not.
  /// The default is "false".
  static void set_print_backtrace(bool enabled);


  /// sets the default output stream for all errors; by default
  /// cerr; if it's null then error output is suppressed.
  static void set_default_stream(std::ostream * ostr);// {
  //  _default_ostr = ostr;
  //}

private:

#ifndef __FJCORE__
#if defined(FASTJET_HAVE_EXECINFO_H) && defined(FASTJET_HAVE_DEMANGLING_SUPPORT)
  /// demangle a given backtrace symbol
  std::string _demangle(const char* symbol);
#endif
#endif

  std::string _message;                ///< error message
  // CMS change: the following are no longer class statics
  //  moved to file static since they were changed to std::atomic
  //  and we still need to allow this header to be parsed by
  //  non C++11 compilers.
  // Change not endorsed by fastjet collaboration 
  //static bool _print_errors;           ///< do we print anything?
  //static bool _print_backtrace;        ///< do we print the backtrace?
  //static std::ostream* _default_ostr; ///< the output stream (cerr if not set)
  // ADDED to commented-out portion 2-Oct-2014 (SRR) :
  // static LimitedWarning _execinfo_undefined;

};


FASTJET_END_NAMESPACE

#endif // __FASTJET_ERROR_HH__
