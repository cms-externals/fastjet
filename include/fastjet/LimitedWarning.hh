#ifndef __FASTJET_LIMITEDWARNING_HH__
#define __FASTJET_LIMITEDWARNING_HH__

//FJSTARTHEADER
// $Id: LimitedWarning.hh 3622 2014-08-13 14:45:45Z salam $
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


#include "fastjet/internal/base.hh"
#include <iostream>
#include <string>
#include <list>
#if __cplusplus >= 201103L
#include <atomic>
#endif

FASTJET_BEGIN_NAMESPACE

/// @ingroup error_handling
/// \class LimitedWarning
/// class to provide facilities for giving warnings up to some maximum
/// number of times and to provide global summaries of warnings that have
/// been issued.
class LimitedWarning {
public:
  
  /// constructor that provides a default maximum number of warnings
  LimitedWarning();

  /// constructor that provides a user-set max number of warnings
  LimitedWarning(int max_warn_in);

  /// outputs a warning to standard error (or the user's default
  /// warning stream if set)
  void warn(const char * warning);

  /// outputs a warning to standard error (or the user's default
  /// warning stream if set)
  void warn(const char * warning) {warn(warning, _default_ostr);}

  /// outputs a warning to standard error (or the user's default
  /// warning stream if set)
  void warn(const std::string & warning) {warn(warning.c_str(), _default_ostr);}

  /// outputs a warning to the specified stream
  void warn(const char * warning, std::ostream * ostr);

  /// outputs a warning to the specified stream
  void warn(const std::string & warning, std::ostream * ostr) {warn(warning.c_str(), ostr);}

  // CMS change: the following two static functions 
  //  are no longer defined in the header.
  // Change not endorsed by fastjet collaboration 

  /// sets the default output stream for all warnings (by default
  /// cerr; passing a null pointer prevents warnings from being output)
  static void set_default_stream(std::ostream * ostr);// {
  //  _default_ostr = ostr;
  //}

  /// sets the default maximum number of warnings of a given kind
  /// before warning messages are silenced.
  static void set_default_max_warn(int max_warn); //{
  //  _max_warn_default = max_warn;
  //}

  /// the maximum number of warning messages that will be printed
  /// by this instance of the class
  int max_warn() const {return _max_warn;}

  /// the number of times so far that a warning has been registered
  /// with this instance of the class.
  int n_warn_so_far() const {return _n_warn_so_far;}

  /// the maximum number of warning messages that will be printed
  /// by this instance of the class
  int max_warn() const {return _max_warn;}

  /// the number of times so far that a warning has been registered
  /// with this instance of the class.
  int n_warn_so_far() const {return _n_warn_so_far;}

  /// returns a summary of all the warnings that came through the
  /// LimiteWarning class
  static std::string summary();

  //std::atomic can not be stored directly in a std::list
  struct atomic_counter {
#if __cplusplus >= 201103L
    std::atomic<unsigned int> _count;
#else
    unsigned int _count;
#endif
    atomic_counter(): _count(0) {}
    atomic_counter(unsigned int iValue): _count(iValue) {}
#if __cplusplus >= 201103L
    atomic_counter( const atomic_counter& iOther): _count(iOther._count.load()) {}
#else
    atomic_counter( const atomic_counter& iOther): _count(iOther._count) {}
#endif

  };
private:
  typedef std::pair<std::string, atomic_counter > Summary;
  const int _max_warn;
  // CMS change: change to std::atomic for thread safety
  // Change not endorsed by fastjet collaboration 
#if __cplusplus >= 201103L
  std::atomic<int> _n_warn_so_far;
  static std::atomic<int> _max_warn_default;
  static std::atomic<std::ostream*> _default_ostr;
  std::atomic<Summary*> _this_warning_summary;
#else
  int _n_warn_so_far;
  //int _max_warn_default;
  //std::ostream* _default_ostr;
  Summary* _this_warning_summary;
#endif
  static std::list< Summary > _global_warnings_summary;
  
};

FASTJET_END_NAMESPACE

#endif // __FASTJET_LIMITEDWARNING_HH__
