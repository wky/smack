#SMACKBench Usage
This document assumes installation has already been performed (see 
[SMACKBench Installation](SMACKBenchInstallation.md)).

For non-emulab installations (generic distributed or single server), skip to the
[Running SMACKBench](#running-smackbench) section. 

##Emulab Swap-In
Currently, there are two methods of installation for SMACK.  A repo based
installation is done by cloning the SMACK project, and running `build.sh`.  A
package based installation is used in svcomp, and is performed by extracting a
tarball.  To facilitate testing of both methods of installation, SMACKBench has
a separate build script for compute nodes, depending on the desired installation
method.

The proper build script must be selected before swapping in the compute nodes.
To do this, 
- From each compute node experiment, select `Modify Experiment`.
- Ensure that the build script corresponding to the desired installation method
  is uncommented, and the other build script is commented.

At this point, the compute node can be swapped in.  A compute node built using
either build script will automatically reboot, at which point SMACK can be
installed.  Once SMACK is installedt, the node is ready to be used.

###Repo-Based SMACK Installation
SMACKBench is currently configured to automatically install SMACK from the
`develop` branch.  Upon swap in and build script completion (which must include
the automatic reboot), the compute node should be ready to use.

###Packaged SMACK Installation
Because the download/access path of the SMACK package to be installed is not
known prior to installation, the package installation process is not automated.

The package must be downloaded and extracted to `/mnt/local/`, and the path to
`smack.sh` added to the environment `PATH`.

An example package installation is:
- Wait for automatic reboot indicating completion of build script.
- `cd /mnt/local/`
- `wget http://soarlab.org/smack/smack-1.5.2-64.tgz`
- `tar -zvxf smack-1.5.2-64.tgz`
- `PATH=$PATH:/mnt/local/smack-1.5.2-64.tgz`

Executing `which smack.sh` after package installation will ensure that
`smack.sh` is available on `PATH`.

##Running SMACKBench
With SMACK properly installed and available on environment `PATH` (`smack.sh`
for packaged install, `smack` for repo install), SMACKBench is ready for use.

There are two modes in which SMACKBench can run, `server` mode, and `run` mode.

###`run` Mode

###`server` Mode
