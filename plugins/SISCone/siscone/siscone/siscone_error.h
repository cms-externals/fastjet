// -*- C++ -*-
///////////////////////////////////////////////////////////////////////////////
// File: siscone_error.h                                                     //
// Description: header file for SISCone error messages (Csiscone_error)      //
// This file is part of the SISCone project.                                 //
// For more details, see http://projects.hepforge.org/siscone                //
//                                                                           //
// Copyright (c) 2006 Gavin Salam and Gregory Soyez                          //
//                                                                           //
// This program is free software; you can redistribute it and/or modify      //
// it under the terms of the GNU General Public License as published by      //
// the Free Software Foundation; either version 2 of the License, or         //
// (at your option) any later version.                                       //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program; if not, write to the Free Software               //
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA //
//                                                                           //
// $Revision:: 327                                                          $//
// $Date:: 2011-11-25 15:19:39 +0100 (Fri, 25 Nov 2011)                     $//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SISCONE_ERROR_H__
#define __SISCONE_ERROR_H__

#include<iostream>
#include<string>

namespace siscone{

/// \class Csiscone_error
/// class corresponding to errors that will be thrown by siscone
class Csiscone_error {
public:
  /// default ctor
  Csiscone_error() {;};

  /// ctor with a given error message
  ///  \param message_in   the error message to be printed
  Csiscone_error(const std::string & message_in) {
    m_message = message_in; 
  // CMS change: hide implementation of printing the error
  // Change not endorsed by fastjet collaboration 
    printMessage(message_in);
  };

  /// access to the error message
  std::string message() const {return m_message;};

  // CMS change: move implementation to source file
  // Change not endorsed by fastjet collaboration 
  /// switch on/off the error message printing
  ///  \param print_errors   errors will be printed when true
  static void setm_print_errors(bool print_errors);// {
  //  m_print_errors = print_errors;};

private:
  // CMS change: hide implementation of printing the error
  // Change not endorsed by fastjet collaboration 
  void printMessage(const std::string& message_in);

  std::string m_message;       ///< the error message
  // CMS change: m_print_errors is no longer a class static
  //  moved to file static since it was changed to std::atomic
  //  and we still need to allow this header to be parsed by
  //  non C++11 compilers.
  // Change not endorsed by fastjet collaboration 
  //static bool m_print_errors;  ///< do we print error messages?
};

}
#endif
