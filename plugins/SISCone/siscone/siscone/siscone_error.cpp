///////////////////////////////////////////////////////////////////////////////
// File: siscone_error.cpp                                                   //
// Description: source file for SISCone error messages (Csiscone_error)      //
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
// $Revision:: 123                                                          $//
// $Date:: 2007-03-01 02:52:16 +0100 (Thu, 01 Mar 2007)                     $//
///////////////////////////////////////////////////////////////////////////////

#include "siscone_error.h"
#if __cplusplus >= 201103L
#include <atomic>
#endif

namespace siscone{

//CMS change: use std::atomic for thread safety
// Change not endorsed by fastjet collaboration
#if __cplusplus >= 201103L
std::atomic<bool> m_print_errors{true};
#else
bool m_print_errors = true;
#endif

void Csiscone_error::setm_print_errors(bool print_errors) {
  m_print_errors = print_errors;
};

void Csiscone_error::printMessage(const std::string& message_in) {
  if (m_print_errors) std::cerr << "siscone::Csiscone_error: "<< message_in << std::endl;
}

}
