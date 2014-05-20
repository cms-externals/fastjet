#!/bin/sh
# Script for generating a minimal config.h file independently of automake
# It must contain a version string, and what else?

configfile=${1:-../include/fastjet/config_auto.h}
echo Generating $configfile
#exit

packname=`grep '^ *AC_INIT' ../configure.ac | sed -e 's/AC_INIT(//' -e 's/\[//g' -e 's/\]//g' -e 's/)//'`

# get the package string
echo '#define FASTJET_PACKAGE_STRING  "'$packname'"' | sed 's/,/ /g' > $configfile

# get the package string
packver=`echo $packname | sed 's/.*,//g'`
echo '#define FASTJET_PACKAGE_VERSION  "'$packver'"'  >> $configfile

# by default some plugins are define/disabled
cat >> $configfile <<EOF

/* The ATLASCone plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_ATLASCONE 

/* The CDFJetClu and CDFMidPoint plugins are enabled by default*/
#define FASTJET_ENABLE_PLUGIN_CDFCONES 

/* The CMSIterativeCone plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_CMSITERATIVECONE 

/* The D0RunICone plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_D0RUNICONE 

/* The D0RunIICone plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_D0RUNIICONE 

/* The EECambridge plugin is enabled by default*/
#define FASTJET_ENABLE_PLUGIN_EECAMBRIDGE 

/* The Jade plugin is enabled by default*/
#define FASTJET_ENABLE_PLUGIN_JADE 

/* The NestedDefs plugin is enabled by default*/
#define FASTJET_ENABLE_PLUGIN_NESTEDDEFS 

/* The PxCone plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_PXCONE

/* The SISCone plugin is enabled by default*/
#define FASTJET_ENABLE_PLUGIN_SISCONE 

/* The TrackJet plugin is disabled by default*/
#undef FASTJET_ENABLE_PLUGIN_TRACKJET

/* The GridJet plugin is enabled by default */
#define FASTJET_ENABLE_PLUGIN_GRIDJET

/* end of plugin section */
EOF
