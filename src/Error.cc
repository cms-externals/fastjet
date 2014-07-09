//STARTHEADER
// $Id: Error.cc 2687 2011-11-14 11:17:51Z soyez $
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
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
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//ENDHEADER

#include "fastjet/Error.hh"
#include "fastjet/config.h"
#include <sstream>
#if __cplusplus >= 201103L
#include<atomic>
#endif

// printing the stack would need execinfo
#ifdef FASTJET_HAVE_EXECINFO_H
#include <execinfo.h>
#include <cstdlib>
#endif

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

//CMS change: use std::atomic for thread safety.
//   Change not endorsed by fastjet collaboration
#if __cplusplus >= 201103L
static std::atomic<bool> _print_errors{true};
static std::atomic<bool> _print_backtrace{false};
static std::atomic<ostream*> _default_ostr{& cerr};
#else
static bool _print_errors = true;
static bool _print_backtrace = false;
static ostream* _default_ostr =& cerr;
#endif

Error::Error(const std::string & message_in) {
  _message = message_in; 
  ostream* ostr = _default_ostr;
  if (_print_errors && ostr){
    ostringstream oss;
    oss << "fastjet::Error:  "<< message_in << endl;

    // only print the stack if execinfo is available and stack enabled
#ifdef FASTJET_HAVE_EXECINFO_H
    if (_print_backtrace){
      void * array[10];
      char ** messages;
 
      int size = backtrace(array, 10);
      messages = backtrace_symbols(array, size);
      
      oss << "stack:" << endl;
      for (int i = 1; i < size && messages != NULL; ++i){
	oss << "  #" << i << ": " << messages[i] << endl;
      }
      free(messages);
    }
#endif

    *ostr << oss.str();
    // get something written to file even 
    // if the program aborts
    ostr->flush(); 

    // // output error message either to cerr or to the user-set stream
    // if (ostr) { *ostr << oss.str();
    //                       // get something written to file even 
    // 			  // if the program aborts
    //                       ostr->flush(); }
    // else               { std::cerr << oss.str(); }
    
  }
}

void Error::set_print_errors(bool print_errors) {_print_errors = print_errors;}

void Error::set_print_backtrace(bool enabled) {_print_backtrace = enabled;}

void Error::set_default_stream(std::ostream * ostr) {
    _default_ostr = ostr;
}

FASTJET_END_NAMESPACE

