//STARTHEADER
// $Id: fastjet_timing_plugins.cc 2806 2011-12-01 17:21:00Z salam $
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


//----------------------------------------------------------------------
/// fastjet_timing.cc: Program to help time and test the fastjet package
/// 
/// It reads files containing multiple events in the format 
/// p1x p1y p1z E1
/// p2x p2y p2z E2
/// ...
/// #END
/// 
/// An example input file containing 10 events is included as 
/// data/Pythia-PtMin1000-LHC-10ev.dat
///
/// Usage:
///   fastjet_timing [-strategy NUMBER] [-repeat nrepeats] [-massive] \
///                  [-combine nevents] [-r Rparameter] [-incl ptmin] [...] \
///                  < data_file
///
/// where the clustering can be repeated to aid timing and multiple
/// events can be combined to get to larger multiplicities. Some options:
///
/// Options for reading
/// -------------------
///
///   -nev     n    number of events to run
///
///   -combine n    for combining multiple events from the data file in order
///                 to get a single high-multipicity event to run.
///
///   -massless     read in only the 3-momenta and deduce energies assuming
///                 that particles are massless
///
///   -dense        adds dense ghost coverage
///
///   -repeat n     repeats each event n times
///
///   -nhardest n   keep only the n hardest particles in the event
///
///   -file name    read from the corresponding file rather than stdin.
///                 (The file will be reopened for each new jet alg.; in
///                 constrast, if you use stdin, each new alg will take a
///                 new event).
/// 
/// Output Options
/// --------------
///
///   -incl ptmin   output of all inclusive jets with pt > ptmin is obtained
///                 with the -incl option.
///
///   -repeat-incl ptmin
///                 same as -incl ptmin but do it for each repetition
///                 of the clustering
///
///   -excld dcut   output of all exclusive jets as obtained in a clustering
///                 with dcut
///
///   -excly ycut   output of all exclusive jets as obtained in a clustering
///                 with ycut
///
///   -excln n      output of clustering to n exclusive jets
///
///   -ee-print     print things as px,py,pz,E
///
///   -get-all-dij  print out all dij values
///   -get-all-yij  print out all yij values
///
///   -const        show jet constituents (works with excl jets)
///
///   -write        for writing out detailed clustering sequence (valuable
///                 for testing purposes)
///
///   -unique_write writes out the sequence of dij's according to the
///                 "unique_history_order" (useful for verifying consistency
///                 between different clustering strategies).
///
///   -root file    sends output to file that can be read in with the script in
///                 root/ so as to show a lego-plot of the event
///
///   -cones        show extra info about internal steps for SISCone
///
///   -area         calculate areas. Additional options include
///                   -area:active
///                   -area:passive
///                   -area:explicit
///                   -area:voronoi Rfact
///                   -area:repeat nrepeat
///                   -ghost-area area
///                   -ghost-maxrap maxrap
///                   -area:fj2               place ghosts as in fj2
///
///   -bkgd         calculate the background density. Additional options include
///                   -bkgd:csab       use the old ClusterSequenceAreaBase methods
///                   -bkgd:jetmedian  use the new JetMedianBackgroundEstimator class
///                   -bkgd:fj2        force jetmedian to calculate sigma as in fj2
///                   -bkgd:gridmedian use GridMedianBackgroundEstimator with grid up to ghost_maxrap-ktR and grid spacing of 2ktR
///
/// Algorithms
/// ----------
///   -all-algs     runs all algorithms
///
///   -kt           switch to the longitudinally invariant kt algorithm
///                 Note: this is the default one.
///
///   -cam          switch to the inclusive Cambridge/Aachen algorithm --
///                 note that the option -excld dcut provides a clustering
///                 up to the dcut which is the minimum squared
///                 distance between any pair of jets.
///
///   -antikt       switch to the anti-kt clustering algorithm
///
///   -genkt        switch to the genkt algorithm
///                 you can provide the parameter of the alg as an argument to 
///                 -genkt (1 by default)
///                 
///   -eekt         switch to the e+e- kt algorithm
///
///   -eegenkt      switch to the genkt algorithm
///                 you can provide the parameter of the alg as an argument to 
///                 -ee_genkt (1 by default)
///                 
/// plugins (don't delete this line)
///
///   -pxcone             switch to the PxCone jet algorithm
/// 
///   -siscone            switch to the SISCone jet algorithm (seedless cones)
///   -sisconespheri      switch to the Spherical SISCone jet algorithm (seedless cones)
///
///   -midpoint           switch to CDF's midpoint code
///   -jetclu             switch to CDF's jetclu code
///
///   -d0runipre96cone    switch to the D0RunIpre96Cone plugin
///   -d0runicone         switch to the D0RunICone plugin
///
///   -d0runiicone        switch to D0's run II midpoint cone
///
///   -trackjet           switch to the TrackJet plugin
///
///   -atlascone          switch to the ATLASCone plugin
///
///   -eecambridge        switch to the EECambridge plugin
///
///   -jade               switch to the Jade plugin
///
///   -cmsiterativecone   switch to the CMSIterativeCone plugin
///
///   -gridjet     switch to the GridJet plugin
///
///  end of plugins (don't delete this line)
///
///
/// Options for running algs
/// ------------------------
///
///   -r            sets the radius of the jet algorithm (default = 1.0)
///
///   -overlap | -f sets the overlap fraction in cone algs with split-merge
///
///   -seed         sets the seed threshold
///
///   -strategy N   indicate stratgey from the enum fastjet::Strategy (see
///                 fastjet/JetDefinition.hh).
///


#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/Selector.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
//#include<cstddef> // for size_t
#include "CmdLine.hh"

// get info on how fastjet was configured
#include "fastjet/config.h"

// include the installed plugins (don't delete this line)
#ifdef FASTJET_ENABLE_PLUGIN_SISCONE
#include "fastjet/SISConePlugin.hh"
#include "fastjet/SISConeSphericalPlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_CDFCONES
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/CDFJetCluPlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_PXCONE
#include "fastjet/PxConePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_D0RUNIICONE
#include "fastjet/D0RunIIConePlugin.hh"
#endif 
#ifdef FASTJET_ENABLE_PLUGIN_TRACKJET
#include "fastjet/TrackJetPlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_ATLASCONE
#include "fastjet/ATLASConePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_EECAMBRIDGE
#include "fastjet/EECambridgePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_JADE
#include "fastjet/JadePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE
#include "fastjet/CMSIterativeConePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_D0RUNICONE
#include "fastjet/D0RunIpre96ConePlugin.hh"
#include "fastjet/D0RunIConePlugin.hh"
#endif
#ifdef FASTJET_ENABLE_PLUGIN_GRIDJET
#include "fastjet/GridJetPlugin.hh"
#endif
// end of installed plugins inclusion (don't delete this line)

using namespace std;

// to avoid excessive typing, define an abbreviation for the 
// fastjet namespace
namespace fj = fastjet;

inline double pow2(const double x) {return x*x;}

// pretty print the jets and their subjets
void print_jets_and_sub (const vector<fj::PseudoJet> & jets, double dcut);

string rootfile;
CmdLine * cmdline_p;

bool do_areas;

/// sort and pretty print jets, with exact behaviour depending on 
/// whether ee_print is true or not
bool ee_print = false;
void print_jets(const vector<fj::PseudoJet> & jets, bool show_const = false);

bool found_unavailable = false;
void is_unavailable(const string & algname) {
  cerr << algname << " requested, but not available for this compilation" << endl;
  found_unavailable = true;
  //exit(0);
}


/// a program to test and time a range of algorithms as implemented or
/// wrapped in fastjet
int main (int argc, char ** argv) {

  fj::ClusterSequence::print_banner();

  CmdLine cmdline(argc,argv);
  cmdline_p = &cmdline;
  // allow the use to specify the fj::Strategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
                                        cmdline.int_val("-clever", fj::Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  int  combine = cmdline.int_val("-combine",1);
  bool write   = cmdline.present("-write");
  bool unique_write = cmdline.present("-unique_write");
  bool hydjet  = cmdline.present("-hydjet");
  double ktR   = cmdline.double_val("-r",1.0);
  ktR   = cmdline.double_val("-R",ktR); // allow -r and -R
  double inclkt = cmdline.double_val("-incl",-1.0);
  double repeatinclkt = cmdline.double_val("-repeat-incl",-1.0);
  int    excln  = cmdline.int_val   ("-excln",-1);
  double excld  = cmdline.double_val("-excld",-1.0);
  double excly  = cmdline.double_val("-excly",-1.0);
  ee_print = cmdline.present("-ee-print");
  bool   get_all_dij   = cmdline.present("-get-all-dij");
  bool   get_all_yij   = cmdline.present("-get-all-yij");
  double subdcut = cmdline.double_val("-subdcut",-1.0);
  double etamax = cmdline.double_val("-etamax",1.0e305);
  bool   show_constituents = cmdline.present("-const");
  bool   massless = cmdline.present("-massless");
  int    nev     = cmdline.int_val("-nev",1);
  bool   add_dense_coverage = cmdline.present("-dense");
  double ghost_maxrap = cmdline.value("-ghost-maxrap",5.0);
  bool   all_algs = cmdline.present("-all-algs");

  fj::Selector particles_sel = (cmdline.present("-nhardest"))
    ? fj::SelectorNHardest(cmdline.value<unsigned int>("-nhardest"))
    : fj::SelectorIdentity();

  do_areas = cmdline.present("-area");
  fj::AreaDefinition area_def;
  if (do_areas) {
    assert(!write); // it's incompatible
    fj::GhostedAreaSpec ghost_spec(ghost_maxrap, 
				   cmdline.value("-area:repeat", 1),
				   cmdline.value("-ghost-area", 0.01));
    if (cmdline.present("-area:fj2")) ghost_spec.set_fj2_placement(true);
    if (cmdline.present("-area:explicit")) {
      area_def = fj::AreaDefinition(fj::active_area_explicit_ghosts, ghost_spec);
    } else if (cmdline.present("-area:passive")) {
      area_def = fj::AreaDefinition(fj::passive_area, ghost_spec);
    } else if (cmdline.present("-area:voronoi")) {
      double Rfact = cmdline.value<double>("-area:voronoi");
      area_def = fj::AreaDefinition(fj::voronoi_area, 
				    fj::VoronoiAreaSpec(Rfact));
    } else {
      cmdline.present("-area:active"); // allow, but do not require, arg
      area_def = fj::AreaDefinition(fj::active_area, ghost_spec);
    }
  }
  bool do_bkgd = cmdline.present("-bkgd"); // background estimation
  bool do_bkgd_csab = false, do_bkgd_jetmedian = false, do_bkgd_fj2 = false;
  bool do_bkgd_gridmedian = false;
  fj::Selector bkgd_range;
  if (do_bkgd) {
    bkgd_range = fj::SelectorAbsRapMax(ghost_maxrap - ktR); 
    if      (cmdline.present("-bkgd:csab"))      {do_bkgd_csab = true;}
    else if (cmdline.present("-bkgd:jetmedian")) {do_bkgd_jetmedian = true;
      do_bkgd_fj2 = cmdline.present("-bkgd:fj2");
    } else if (cmdline.present("-bkgd:gridmedian")) {do_bkgd_gridmedian = true;
    } else {
      throw fj::Error("with the -bkgd option, some particular background must be specified (csab or jetmedian)");
    }
    assert(do_areas || do_bkgd_gridmedian);
  }

  bool show_cones = cmdline.present("-cones"); // only works for siscone

  // for cone algorithms
  // allow -f and -overlap
  double overlap_threshold = cmdline.double_val("-overlap",0.5);
  overlap_threshold = cmdline.double_val("-f",overlap_threshold); 
  double seed_threshold = cmdline.double_val("-seed",1.0);

  // for ee algorithms, allow to specify ycut
  double ycut = cmdline.double_val("-ycut",0.08);

  // for printing jets to a file for reading by root
  rootfile = cmdline.value<string>("-root","");

  // out default scheme is the E_scheme
  fj::RecombinationScheme scheme = fj::E_scheme;

  // The following option causes the Cambridge algo to be used.
  // Note that currently the only output that works sensibly here is
  // "-incl 0"
  vector<fj::JetDefinition> jet_defs;
  if (all_algs || cmdline.present("-cam") || cmdline.present("-CA")) {
    jet_defs.push_back( fj::JetDefinition(fj::cambridge_algorithm, ktR, scheme, strategy));
  } 
  if (all_algs || cmdline.present("-antikt")) {
    jet_defs.push_back( fj::JetDefinition(fj::antikt_algorithm, ktR, scheme, strategy));
  } 
  if (all_algs || cmdline.present("-genkt")) {
    double p;
    if (cmdline.present("-genkt")) p = cmdline.value<double>("-genkt");
    else                           p = -0.5;
    jet_defs.push_back( fj::JetDefinition(fj::genkt_algorithm, ktR, p, scheme, strategy));
  } 
  if (all_algs || cmdline.present("-eekt")) {
    jet_defs.push_back( fj::JetDefinition(fj::ee_kt_algorithm));
  } 
  if (all_algs || cmdline.present("-eegenkt")) {
    double p;
    if (cmdline.present("-eegenkt")) p = cmdline.value<double>("-eegenkt");
    else                             p = -0.5;
    jet_defs.push_back( fj::JetDefinition(fj::ee_genkt_algorithm, ktR, p, scheme, strategy));

// checking if one asks to run a plugin (don't delete this line)
  } 
  if (all_algs || cmdline.present("-midpoint")) {
#ifdef FASTJET_ENABLE_PLUGIN_CDFCONES
    typedef fj::CDFMidPointPlugin MPPlug; // for brevity
    double cone_area_fraction = 1.0;
    int    max_pair_size = 2;
    int    max_iterations = 100;
    MPPlug::SplitMergeScale sm_scale = MPPlug::SM_pt;
    if (cmdline.present("-sm-pttilde")) sm_scale = MPPlug::SM_pttilde;
    if (cmdline.present("-sm-pt")) sm_scale = MPPlug::SM_pt; // default
    if (cmdline.present("-sm-mt")) sm_scale = MPPlug::SM_mt;
    if (cmdline.present("-sm-Et")) sm_scale = MPPlug::SM_Et;
    jet_defs.push_back( fj::JetDefinition( new fj::CDFMidPointPlugin (
                                      seed_threshold, ktR, 
                                      cone_area_fraction, max_pair_size,
                                      max_iterations, overlap_threshold,
                                      sm_scale)));
#else  // FASTJET_ENABLE_PLUGIN_CDFCONES
    is_unavailable("midpoint");
#endif // FASTJET_ENABLE_PLUGIN_CDFCONES
  } 
  if (all_algs || cmdline.present("-pxcone")) {
#ifdef FASTJET_ENABLE_PLUGIN_PXCONE
    double min_jet_energy = 5.0;
    jet_defs.push_back( fj::JetDefinition( new fj::PxConePlugin (
                                      ktR, min_jet_energy,
                                      overlap_threshold)));
#else  // FASTJET_ENABLE_PLUGIN_PXCONE
    is_unavailable("pxcone");
#endif // FASTJET_ENABLE_PLUGIN_PXCONE
  } 
  if (all_algs || cmdline.present("-jetclu")) {
#ifdef FASTJET_ENABLE_PLUGIN_CDFCONES
    jet_defs.push_back( fj::JetDefinition( new fj::CDFJetCluPlugin (
                                                                    ktR, overlap_threshold, seed_threshold)));
#else  // FASTJET_ENABLE_PLUGIN_CDFCONES
    is_unavailable("pxcone");
#endif // FASTJET_ENABLE_PLUGIN_CDFCONES
  } 
  if (all_algs || cmdline.present("-siscone") || cmdline.present("-sisconespheri")) {
#ifdef FASTJET_ENABLE_PLUGIN_SISCONE
    typedef fj::SISConePlugin SISPlug; // for brevity
    int npass = cmdline.value("-npass",0);
    if (all_algs || cmdline.present("-siscone")) {
      double sisptmin = cmdline.value("-sisptmin",0.0);
      SISPlug * plugin = new SISPlug (ktR, overlap_threshold,npass,sisptmin);
      if (cmdline.present("-sm-pt")) plugin->set_split_merge_scale(SISPlug::SM_pt);
      if (cmdline.present("-sm-mt")) plugin->set_split_merge_scale(SISPlug::SM_mt);
      if (cmdline.present("-sm-Et")) plugin->set_split_merge_scale(SISPlug::SM_Et);
      if (cmdline.present("-sm-pttilde")) plugin->set_split_merge_scale(SISPlug::SM_pttilde);
      // cause it to use the jet-definition's own recombiner
      plugin->set_use_jet_def_recombiner(true);
      jet_defs.push_back( fj::JetDefinition(plugin));
    } 
    if (all_algs || cmdline.present("-sisconespheri")) {
      double sisEmin = cmdline.value("-sisEmin",0.0);
      fj::SISConeSphericalPlugin * plugin = 
	new fj::SISConeSphericalPlugin(ktR, overlap_threshold,npass,sisEmin);
      if (cmdline.present("-ghost-sep")) {
	plugin->set_ghost_separation_scale(cmdline.value<double>("-ghost-sep"));
      }
      jet_defs.push_back( fj::JetDefinition(plugin));
    }
#else  // FASTJET_ENABLE_PLUGIN_SISCONE
    is_unavailable("siscone");
#endif // FASTJET_ENABLE_PLUGIN_SISCONE
  } 
  if (all_algs || cmdline.present("-d0runiicone")) {
#ifdef FASTJET_ENABLE_PLUGIN_D0RUNIICONE
    double min_jet_Et = 6.0; // was 8 GeV in earlier work
    jet_defs.push_back( fj::JetDefinition(new fj::D0RunIIConePlugin(ktR,min_jet_Et)));
#else  // FASTJET_ENABLE_PLUGIN_D0RUNIICONE
    is_unavailable("D0RunIICone");
#endif // FASTJET_ENABLE_PLUGIN_D0RUNIICONE
  } 
  if (all_algs || cmdline.present("-trackjet")) {
#ifdef FASTJET_ENABLE_PLUGIN_TRACKJET
    jet_defs.push_back( fj::JetDefinition(new fj::TrackJetPlugin(ktR)));
#else  // FASTJET_ENABLE_PLUGIN_TRACKJET
    is_unavailable("TrackJet");
#endif // FASTJET_ENABLE_PLUGIN_TRACKJET
  } 
  if (all_algs || cmdline.present("-atlascone")) {
#ifdef FASTJET_ENABLE_PLUGIN_ATLASCONE
    jet_defs.push_back( fj::JetDefinition(new fj::ATLASConePlugin(ktR)));
#else  // FASTJET_ENABLE_PLUGIN_ATLASCONE
    is_unavailable("ATLASCone");
#endif // FASTJET_ENABLE_PLUGIN_ATLASCONE
  } 
  if (all_algs || cmdline.present("-eecambridge")) {
#ifdef FASTJET_ENABLE_PLUGIN_EECAMBRIDGE
    jet_defs.push_back( fj::JetDefinition(new fj::EECambridgePlugin(ycut)));
#else  // FASTJET_ENABLE_PLUGIN_EECAMBRIDGE
    is_unavailable("EECambridge");
#endif // FASTJET_ENABLE_PLUGIN_EECAMBRIDGE
  } 
  if (all_algs || cmdline.present("-jade")) {
#ifdef FASTJET_ENABLE_PLUGIN_JADE
    jet_defs.push_back( fj::JetDefinition(new fj::JadePlugin()));
#else  // FASTJET_ENABLE_PLUGIN_JADE
    is_unavailable("Jade");
#endif // FASTJET_ENABLE_PLUGIN_JADE
  } 
  if (all_algs || cmdline.present("-cmsiterativecone")) {
#ifdef FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE
    jet_defs.push_back( fj::JetDefinition(new fj::CMSIterativeConePlugin(ktR,seed_threshold)));
#else  // FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE
    is_unavailable("CMSIterativeCone");
#endif // FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE
  } 
  if (all_algs || cmdline.present("-d0runipre96cone")) {
#ifdef FASTJET_ENABLE_PLUGIN_D0RUNICONE
    jet_defs.push_back( fj::JetDefinition(new fj::D0RunIpre96ConePlugin(ktR, seed_threshold, overlap_threshold)));
#else  // FASTJET_ENABLE_PLUGIN_D0RUNICONE
    is_unavailable("D0RunICone");
#endif // FASTJET_ENABLE_PLUGIN_D0RUNICONE
  } 
  if (all_algs || cmdline.present("-d0runicone")) {
#ifdef FASTJET_ENABLE_PLUGIN_D0RUNICONE
    jet_defs.push_back( fj::JetDefinition(new fj::D0RunIConePlugin(ktR, seed_threshold, overlap_threshold)));
#else  // FASTJET_ENABLE_PLUGIN_D0RUNICONE
    is_unavailable("D0RunICone");
#endif // FASTJET_ENABLE_PLUGIN_D0RUNICONE
  } 
  if (all_algs || cmdline.present("-gridjet")) {
#ifdef FASTJET_ENABLE_PLUGIN_GRIDJET
    // we want a grid_ymax of 5.0, but when using R=0.4 (i.e. grid
    // spacing of 0.8), this leads to 12.5 grid cells; depending on
    // whether this is 12.499999999999 or 12.5000000....1 this gets
    // converted either to 12 or 13, making the results sensitive to
    // rounding errors.
    //
    // Instead we therefore take 4.9999999999, which avoids this problem.
    double grid_ymax = 4.9999999999;
    jet_defs.push_back( fj::JetDefinition(new fj::GridJetPlugin(grid_ymax, ktR*2.0)));
#else  // FASTJET_ENABLE_PLUGIN_GRIDJET
    is_unavailable("GridJet");
#endif // FASTJET_ENABLE_PLUGIN_GRIDJET
// end of checking if one asks to run a plugin (don't delete this line)
  } 
  if (all_algs || 
      cmdline.present("-kt") || 
      (jet_defs.size() == 0 && !found_unavailable))  {
    jet_defs.push_back( fj::JetDefinition(fj::kt_algorithm, ktR, strategy));
  }

  string filename = cmdline.value<string>("-file", "");


  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options were not recognized"<<endl; 
    exit(-1);}

  for (unsigned idef = 0; idef < jet_defs.size(); idef++) {
  fj::JetDefinition & jet_def = jet_defs[idef];
  istream * istr;
  if (filename == "") istr = &cin;
  else                istr = new ifstream(filename.c_str());

  for (int iev = 0; iev < nev; iev++) {
  vector<fj::PseudoJet> jets;
  vector<fj::PseudoJet> particles;
  string line;
  int  ndone = 0;
  while (getline(*istr, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {
      ndone += 1;
      if (ndone == combine) {break;}
    }
    if (line.substr(0,1) == "#") {continue;}
    valarray<double> fourvec(4);
    if (hydjet) {
      // special reading from hydjet.txt event record (though actually
      // this is supposed to be a standard pythia event record, so
      // being able to read from it is perhaps not so bad an idea...)
      int ii, istat,id,m1,m2,d1,d2;
      double mass;
      linestream >> ii>> istat >> id >> m1 >> m2 >> d1 >> d2
		 >> fourvec[0] >> fourvec[1] >> fourvec[2] >> mass;
      // current file contains mass of particle as 4th entry
      if (istat == 1) {
	fourvec[3] = sqrt(+pow2(fourvec[0])+pow2(fourvec[1])
			  +pow2(fourvec[2])+pow2(mass));
      }
    } else {
      if (massless) {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2];
	fourvec[3] = sqrt(pow2(fourvec[0])+pow2(fourvec[1])+pow2(fourvec[2]));}
      else {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3];
      }
    }
    fj::PseudoJet psjet(fourvec);
    if (abs(psjet.rap() < etamax)) {particles.push_back(psjet);}
  }

  // add a fake underlying event which is very soft, uniformly distributed
  // in eta,phi so as to allow one to reconstruct the area that is associated
  // with each jet.
  if (add_dense_coverage) {
    fj::GhostedAreaSpec ghosted_area_spec(ghost_maxrap);
    //fj::GhostedAreaSpec ghosted_area_spec(-2.0,4.0); // asymmetric range
    // for plots, reduce the scatter default of 1, to avoid "holes"
    // in the subsequent calorimeter view
    ghosted_area_spec.set_grid_scatter(0.5); 
    ghosted_area_spec.add_ghosts(particles);
    //----- old code ------------------
    // srand(2);
    // int nphi = 60;
    // int neta = 100;
    // double kt = 1e-1;
    // for (int iphi = 0; iphi<nphi; iphi++) {
    //   for (int ieta = -neta; ieta<neta+1; ieta++) {
    // 	double phi = (iphi+0.5) * (fj::twopi/nphi) + rand()*0.001/RAND_MAX;
    // 	double eta = ieta * (10.0/neta)  + rand()*0.001/RAND_MAX;
    // 	kt = 1e-20*(1+rand()*0.1/RAND_MAX);
    // 	double pminus = kt*exp(-eta);
    // 	double pplus  = kt*exp(+eta);
    // 	double px = kt*sin(phi);
    // 	double py = kt*cos(phi);
    // 	//cout << kt<<" "<<eta<<" "<<phi<<"\n";
    // 	fj::PseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
    // 	particles.push_back(mom);
    //   }
    // }
  }

  // select the particles that pass the selection cut
  particles = particles_sel(particles);
  
  for (int irepeat = 0; irepeat < repeat ; irepeat++) {
    int nparticles = particles.size();
    try {
    auto_ptr<fj::ClusterSequence> clust_seq;
    if (do_areas) {
      clust_seq.reset(new fj::ClusterSequenceArea(particles,jet_def,area_def));
    } else {
      clust_seq.reset(new fj::ClusterSequence(particles,jet_def,write));
    }

    // repetitive output
    if (repeatinclkt >= 0.0) {
      vector<fj::PseudoJet> jets_local = sorted_by_pt(clust_seq->inclusive_jets(repeatinclkt));
    }

    if (irepeat != 0) {continue;}
    cout << "iev "<<iev<< ": number of particles = "<< nparticles << endl;
    cout << "strategy used =  "<< clust_seq->strategy_string()<< endl;
    if (iev == 0) cout << "Jet Definition: " << jet_def.description() << " (" << fj::fastjet_version_string() << ")" << endl;
    if (do_areas && iev == 0) cout << "Area definition: " << area_def.description() << endl;

    // now provide some nice output...
    if (inclkt >= 0.0) {
      vector<fj::PseudoJet> jets_local = sorted_by_pt(clust_seq->inclusive_jets(inclkt));
      print_jets(jets_local, show_constituents);

    }

    if (excln > 0) {
      cout << "Printing "<<excln<<" exclusive jets\n";
      print_jets(clust_seq->exclusive_jets(excln), show_constituents);
    }

    if (excld > 0.0) {
      cout << "Printing exclusive jets for d = "<<excld<<"\n";
      print_jets(clust_seq->exclusive_jets(excld), show_constituents);
    }

    if (excly > 0.0) {
      cout << "Printing exclusive jets for ycut = "<<excly<<"\n";
      print_jets(clust_seq->exclusive_jets_ycut(excly), show_constituents);
    }

    if (get_all_dij) {
      for (int i = nparticles-1; i >= 0; i--) {
        printf("d for n = %4d -> %4d is %14.5e\n", i+1, i, clust_seq->exclusive_dmerge(i));
      }
    }
    if (get_all_yij) {
      for (int i = nparticles-1; i >= 0; i--) {
        printf("y for n = %4d -> %4d is %14.5e\n", i+1, i, clust_seq->exclusive_ymerge(i));
      }
    }

    // have the option of printing out the subjets (at scale dcut) of
    // each inclusive jet
    if (subdcut >= 0.0) {
      print_jets_and_sub(clust_seq->inclusive_jets(), subdcut);
    }
    
    // useful for testing that recombination sequences are unique
    if (unique_write) {
      vector<int> unique_history = clust_seq->unique_history_order();
      // construct the inverse of the above mapping
      vector<int> inv_unique_history(clust_seq->history().size());
      for (unsigned int i = 0; i < unique_history.size(); i++) {
	inv_unique_history[unique_history[i]] = i;}

      for (unsigned int i = 0; i < unique_history.size(); i++) {
	fj::ClusterSequence::history_element el = 
	  clust_seq->history()[unique_history[i]];
	int uhp1 = el.parent1>=0 ? inv_unique_history[el.parent1] : el.parent1;
	int uhp2 = el.parent2>=0 ? inv_unique_history[el.parent2] : el.parent2;
	printf("%7d u %15.8e %7d u %7d u\n",i,el.dij,uhp1, uhp2);
      }
    }


#ifdef FASTJET_ENABLE_PLUGIN_SISCONE
    // provide some complementary information for SISCone 
    if (show_cones) {
      const fj::SISConeExtras * extras = 
        dynamic_cast<const fj::SISConeExtras *>(clust_seq->extras());
      cout << "most ambiguous split (difference in squared dist) = "
           << extras->most_ambiguous_split() << endl;
      vector<fastjet::PseudoJet> stable_cones(extras->stable_cones()); 
      stable_cones = sorted_by_rapidity(stable_cones);
      for (unsigned int i = 0; i < stable_cones.size(); i++) {
      //if (stable_cones[i].phi() < 5.0 && stable_cones[i].phi() > 4.0) {
        printf("%5u %15.8f %15.8f %15.8f\n",
               i,stable_cones[i].rap(),stable_cones[i].phi(),
               stable_cones[i].perp() );
      //}
      }
      
      // also show passes for jets
      vector<fj::PseudoJet> sisjets = clust_seq->inclusive_jets();
      printf("\n%15s %15s %15s %12s %8s %8s\n","rap","phi","pt","user-index","pass","nconst");
      for (unsigned i = 0; i < sisjets.size(); i++) {
        printf("%15.8f %15.8f %15.8f %12d %8d %8u\n",
               sisjets[i].rap(), sisjets[i].phi(), sisjets[i].perp(), 
	       sisjets[i].user_index(), extras->pass(sisjets[i]),
	       (unsigned int) clust_seq->constituents(sisjets[i]).size()
	       );
	
      }
    }
#endif // FASTJET_ENABLE_PLUGIN_SISCONE

    if (do_bkgd) {
      double rho, sigma, mean_area, empty_area, n_empty_jets;
      fj::ClusterSequenceAreaBase * csab = 
	dynamic_cast<fj::ClusterSequenceAreaBase *>(clust_seq.get());
      if (do_bkgd_csab) {
	csab->get_median_rho_and_sigma(bkgd_range, true, rho, sigma, mean_area);
	empty_area = csab->empty_area(bkgd_range);
	n_empty_jets = csab->n_empty_jets(bkgd_range);
      } else if (do_bkgd_jetmedian) {
	fj::JetMedianBackgroundEstimator bge(bkgd_range);
	bge.set_provide_fj2_sigma(do_bkgd_fj2);
	bge.set_cluster_sequence(*csab);
	rho = bge.rho();
	sigma = bge.sigma();
	mean_area = bge.mean_area();
	empty_area = bge.empty_area();
	n_empty_jets = bge.n_empty_jets();
      } else {
	assert(do_bkgd_gridmedian);
        double rapmin, rapmax;
        bkgd_range.get_rapidity_extent(rapmin, rapmax);
	fj::GridMedianBackgroundEstimator bge(rapmax, 2*ktR);
        bge.set_particles(particles);
        rho = bge.rho();
        sigma = bge.sigma();
        mean_area = bge.mean_area();
        empty_area = 0;
        n_empty_jets = 0;
      }
      cout << "  rho = " << rho 
	   << ", sigma = " << sigma 
	   << ", mean_area = " << mean_area
	   << ", empty_area = " << empty_area
	   << ", n_empty_jets = " << n_empty_jets
	   << endl;
    }
  } // try
  catch (fastjet::Error fjerr) {
    cout << "Caught fastjet error, exiting gracefully" << endl;
    exit(0);
  }

  } // irepeat
  } // iev
  // if we've instantiated a plugin, delete it
  if (jet_def.strategy()==fj::plugin_strategy){
    delete jet_def.plugin();
  }
  // close any file that we've opened
  if (istr != &cin) delete istr;
  } // jet_defs

}




//------ HELPER ROUTINES -----------------------------------------------
/// print a single jet
void print_jet (const fj::PseudoJet & jet) {
  unsigned int n_constituents = jet.constituents().size();
  printf("%15.8f %15.8f %15.8f %8u\n",
         jet.rap(), jet.phi(), jet.perp(), n_constituents);
}


//----------------------------------------------------------------------
void print_jets(const vector<fj::PseudoJet> & jets_in, bool show_constituents) {
  vector<fj::PseudoJet> jets;
  if (ee_print) {
    jets = sorted_by_E(jets_in);
    for (unsigned int j = 0; j < jets.size(); j++) {
      printf("%5u %15.8f %15.8f %15.8f %15.8f\n",
	     j,jets[j].px(),jets[j].py(),jets[j].pz(),jets[j].E());
      if (show_constituents) {
	vector<fj::PseudoJet> const_jets = jets[j].constituents();
	for (unsigned int k = 0; k < const_jets.size(); k++) {
	  printf("        jet%03u %15.8f %15.8f %15.8f %15.8f\n",j,const_jets[k].px(),
		 const_jets[k].py(),const_jets[k].pz(),const_jets[k].E());
	}
	cout << "\n\n";
    }

    }
  } else {
    jets = sorted_by_pt(jets_in);
    for (unsigned int j = 0; j < jets.size(); j++) {
      printf("%5u %15.8f %15.8f %15.8f",
	     j,jets[j].rap(),jets[j].phi(),jets[j].perp());
      // also print out the scalar area and the perp component of the
      // 4-vector (just enough to check a reasonable 4-vector?)
      if (do_areas) printf(" %15.8f %15.8f", jets[j].area(),
			                     jets[j].area_4vector().perp());
      cout << "\n";

      if (show_constituents) {
	vector<fj::PseudoJet> const_jets = jets[j].constituents();
	for (unsigned int k = 0; k < const_jets.size(); k++) {
	  printf("        jet%03u %15.8f %15.8f %15.8f %5d\n",j,const_jets[k].rap(),
		 const_jets[k].phi(),sqrt(const_jets[k].kt2()), const_jets[k].cluster_hist_index());
	}
	cout << "\n\n";
      }
    }
  }

  if (rootfile != "") {
    ofstream ostr(rootfile.c_str());
    ostr << "# " << cmdline_p->command_line() << endl;
    ostr << "# output for root" << endl;
    assert(jets.size() > 0);
    jets[0].validated_cs()->print_jets_for_root(jets,ostr);
  }

}


//----- SUBJETS --------------------------------------------------------
/// a function that pretty prints a list of jets and the subjets for each
/// one
void print_jets_and_sub (const vector<fj::PseudoJet> & jets, double dcut) {

  // sort jets into increasing pt
  vector<fj::PseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("Printing jets and their subjets with subdcut = %10.5f\n",dcut);
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
	 "phi", "pt", "n constituents");

  // have various kinds of subjet finding, to test consistency among them
  enum SubType {internal, newclust_dcut, newclust_R};
  SubType subtype = internal;
  //SubType subtype = newclust_dcut;
  //SubType subtype = newclust_R;

  // print out the details for each jet
  //for (unsigned int i = 0; i < sorted_jets.size(); i++) {
  for (vector<fj::PseudoJet>::const_iterator jet = sorted_jets.begin(); 
       jet != sorted_jets.end(); jet++) {
    const fj::JetDefinition & jet_def = jet->validated_cs()->jet_def();

    // if jet pt^2 < dcut with kt alg, then some methods of
    // getting subjets will return nothing -- so skip the jet
    if (jet_def.jet_algorithm() == fj::kt_algorithm 
        && jet->perp2() < dcut) continue;


    printf("%5u       ",(unsigned int) (jet - sorted_jets.begin()));
    print_jet(*jet);
    vector<fj::PseudoJet> subjets;
    fj::ClusterSequence * cspoint;
    if (subtype == internal) {
      cspoint = 0;
      subjets = jet->exclusive_subjets(dcut);
      double ddnp1 = jet->exclusive_subdmerge_max(subjets.size());
      double ddn   = jet->exclusive_subdmerge_max(subjets.size()-1);
      cout << "     for " << ddnp1 << " < d < " << ddn << " one has " << endl;
    } else if (subtype == newclust_dcut) {
      cspoint = new fj::ClusterSequence(jet->constituents(), jet_def);
      subjets = cspoint->exclusive_jets(dcut);
    } else if (subtype == newclust_R) {
      assert(jet_def.jet_algorithm() == fj::cambridge_algorithm);
      fj::JetDefinition subjd(jet_def.jet_algorithm(), jet_def.R()*sqrt(dcut));
      cspoint = new fj::ClusterSequence(jet->constituents(), subjd);
      subjets = cspoint->inclusive_jets();
    } else {
      cerr << "unrecognized subtype for subjet finding" << endl;
      exit(-1);
    }

    subjets = sorted_by_pt(subjets);
    for (unsigned int j = 0; j < subjets.size(); j++) {
      printf("    -sub-%02u ",j);
      print_jet(subjets[j]);
    }

    if (cspoint != 0) delete cspoint;

    //fj::ClusterSequence subseq(clust_seq->constituents(sorted_jets[i]),
    //                          fj::JetDefinition(fj::cambridge_algorithm, 0.4));
    //vector<fj::PseudoJet> subjets = sorted_by_pt(subseq.inclusive_jets());
    //for (unsigned int j = 0; j < subjets.size(); j++) {
    //  printf("    -sub-%02u ",j);
    //  print_jet(subseq, subjets[j]);
    //}
  }

}

