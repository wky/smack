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

There are two modes in which SMACKBench can run, `run` mode, and `server` mode.

<Generic info here, about svcomp sets, input xml files, etc.  Witcheck>

###`run` Mode
`run` mode is for executing a single SVCOMP benchmark set.  This allows a
specific set to be executed, and the stdout of SMACKBench is routed to the
console.

`SMACKBench.py` should be executed from the SMACKBench installation directory
(`/proj/<projectName>/SMACKBenchResults/` on emulab, as per
[SMACKBench Installation](SMACKBenchInstallation.md).  Adjust accordingly).

```
usage: SMACKBench.py run [-h] -s SET -x FILE [-r NUM] [-m NUM] [-c FILE]
                         [-d DESC]

optional arguments:
  -h, --help            show this help message and exit
  -s SET, --svcomp-set SET
                        The svcomp set to execute (default: None)
  -x FILE, --inputXmlFile FILE
                        The input XML file with which to run svcomp-set
                        (default: None)
  -r NUM, --concurrentRuns NUM
                        The number of concurrent benchmarks to run (default:
                        8)
  -m NUM, --memoryPerRun NUM
                        Amount of memory per concurrent run (in MB) (default:
                        15000)
  -c FILE, --config-file FILE
                        The json file with SMACKBench config settings
                        (default: inputFiles/config.json)
  -d DESC, --description DESC
                        A description field (identifier) to be associated with
                        each set (default: )
```

An example call providing only the required switches (that is, accepting the
default options where available) might be:
`./SMACKBench.py -s Simple -x
inputFiles/inputXMLFiles/svcomp_m32_witcheck_packaged.xml`

###`server` Mode
In `server` mode, SMACKBench watches a queue file on the shared storage.  The
queue file consists of "<setName> <inputXMLFile>" pairs (one pair per line,
space separated).  If there is an entry in the queue file, one is popped off the
top of the list and executed in a fashion similar to `run` mode, using the
options that were chosen when server mode was started.  This allows multiple
compute nodes to work towards completing a large list of SVCOMP sets.

To populate the queue, two templates exist.  The contents of these templates 
should be copied into the actual queue file.  `inputFiles/queueStd` contains the
standard queue template for repo based installations of SMACK.
`inputFiles/queueStdPackaged` contains the standard queue template for packaged
based installations.  Both templates provide a queue that will run all SVCOMP
sets that were used in SVCOMP 2016.

In server mode, stdout and stderr are redirected to a log file, which can be
viewed for troubleshooting or checking progress (currently, by default, they go
to `<installRoot>/data/serverLogs/`).

To stop `server` mode, simply execute `./SMACKBench.py stop`.  (Note, this will
also stop SMACKBench in `run` mode.)

```
usage: SMACKBench.py server [-h] [-q FILE] [-r NUM] [-m NUM] [-c FILE]
                            [-d DESC]

optional arguments:
  -h, --help            show this help message and exit
  -q FILE, --queue-file FILE
                        The file containing the list of jobs to process
                        (default: inputFiles/queue)
  -r NUM, --concurrentRuns NUM
                        The number of concurrent benchmarks to run (default:
                        8)
  -m NUM, --memoryPerRun NUM
                        Amount of memory per concurrent run (in MB) (default:
                        15000)
  -c FILE, --config-file FILE
                        The json file with SMACKBench config settings
                        (default: inputFiles/config.json)
  -d DESC, --description DESC
                        A description field (identifier) to be associated with
                        each set (default: )
```
