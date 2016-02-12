#!/bin/bash

# MAJOR SYSTEM CHANGES:
#  -Installs cgroups, modifies grub, requires restart
#  -Installs java7, and sets this to be primary java executable
#  -Installs ant

#Default directory is <parent of smack project>/SMACKBench
INSTALLDIR=../../../SMACKBench
SRCDIR=`pwd`

#Gets rid of installation
if [[ $1 == "clean" ]]
    then
    #Check for alternate install dir to clean
    if [[ ! -z $2 ]] 
    then
	#Strip trailing slash, if any
	INSTALLDIR=${2%/}
    fi
    rm ${INSTALLDIR} -rf
    echo SMACKBench Install Removed
    echo
    exit
fi

#Gets rid of everything except sv-benchmarks, since they're big
if [[ $1 == "tidy" ]]
    then
    #Check for alternate install dir to tidy
    if [[ ! -z $2 ]] 
    then
	#Strip trailing slash, if any
	INSTALLDIR=${2%/}
    fi
    rm -rf ${INSTALLDIR}/benchexec/
    rm -f  ${INSTALLDIR}/data/*.py
    rm -rf ${INSTALLDIR}/data/serverLogs/
    rm -rf ${INSTALLDIR}/data/runs/
    rm -rf ${INSTALLDIR}/data/__pycache__/
    rm -rf ${INSTALLDIR}/inputFiles/
    rm -f  ${INSTALLDIR}/runSMACKBench.sh
    rm -rf ${INSTALLDIR}/cpachecker/
    echo "SMACKBench Install Removed (except for svcomp benchmarks)"
    echo
    exit
fi

#Copies things over again
if [[ $1 == "refresh" ]]
    then
    #Check for alternate install dir to refresh
    if [[ ! -z $2 ]] 
    then
	#Strip trailing slash, if any
	INSTALLDIR=${2%/}
    fi
    cp src/* ${INSTALLDIR} -r
    echo SMACKBench Installation Refreshed
    echo
    exit
fi

#Compares overlay files with those in target install dir
if [[ $1 == "compare" ]]
    then
    #Check for alternate install dir to compare
    if [[ ! -z $2 ]] 
    then
	#Strip trailing slash, if any
	INSTALLDIR=${2%/}
    fi
    for f in $(cd src; find . -type f; cd ..)
    do
	echo
	echo
	echo ----------------------
	echo $f
	echo ----------------------
	diff ./src/${f:2} ${INSTALLDIR}/${f:2} -s
    done
    exit
fi


#Set install dir if first argument is not empty string 
#  (or "clean" or "tidy" or "refresh")
if [[ ! -z $1 ]] 
then
    #Strip trailing slash, if any
    INSTALLDIR=${1%/}
fi


##########################
# Setup cgroups
##########################
#In ubuntu 14+, this cgroup-bin seems to properly mount the various
#  copies of cgroup for each category.
#sudo apt-get update
#sudo apt-get install cgroup-bin -y
#And then restart
#However, for BenchExec's actual documentation, it says to do the following:
#sudo mount -t cgroup cgroup /sys/fs/cgroup
#sudo chmod o+wt,g+w /sys/fs/cgroup/
#Also, must add "swapaccount=1" to GRUB_CMDLINE_LINUX option in /etc/default/grub
#   Then run 
#   sudo update-grub
#   Then restart
#This is by far the goofiest thing with this setup.  I've only done it once,
#   and I don't know what other issues will be encountered when done again.


##########################
# Setup install folder
##########################
mkdir -p ${INSTALLDIR}

##########################
# Get svcomp benchmarks
##########################
#Download svcomp benchmarks
git clone https://github.com/smackers/sv-benchmarks.git ${INSTALLDIR}/data/sv-benchmarks

##########################
# Prepare benchexec
##########################
#Get BenchExec package
git clone https://github.com/sosy-lab/benchexec.git ${INSTALLDIR}/benchexec/
#And its dependency, tempita
wget https://pypi.python.org/packages/3.3/T/Tempita/Tempita-0.5.3dev-py3.3.egg --directory-prefix=${INSTALLDIR}
#Install unzip if missing
if [[ -z `which unzip` ]]
then
    sudo apt-get install unzip -y
fi
#The following extracts only the Tempita-0.5.3dev/tempita folder, and when it does so,
#  replaces the 'Tempita-0.5.3dev/' portion with 'benchexec/'.
#  In other words, it extracts just the module portion of tempita to the benchexec folder
#  -n means don't overwrite existing target files, tempita/\* means get only files in tempita dir
unzip -n ${INSTALLDIR}/Tempita-0.5.3dev-py3.3.egg tempita/\* -d ${INSTALLDIR}/benchexec/
rm ${INSTALLDIR}/Tempita-0.5.3dev-py3.3.egg
#Copy smack's BenchExec wrapper to the benchexec installation
cp src/* ${INSTALLDIR} -r


################################
# Prepare CPAchecker as witness
################################
mkdir ${INSTALLDIR}/cpachecker
cd ${INSTALLDIR}/cpachecker
wget http://sv-comp.sosy-lab.org/2016/witnesses/witness-validation.zip
unzip witness-validation.zip
cd ${SRCDIR}
