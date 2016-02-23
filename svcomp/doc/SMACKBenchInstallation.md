#SMACKBench Installation

###A Note on Hardware

SMACKBench was originally designed for use on any machine, however, most of the
recent effort has gone towards making SMACKBench work well in a distributed
[emulab](http://emulab.net/) environment.  As a result, the scripted
installation procedures focus on setup within this environment, and the single
server installation scripts are not kept up to date.

With that said, a small section can be found near the end of this document with
guidelines for patching together a SMACKBench installation on a single machine.

##Distributed Installation
###Overview
SMACKBench has been designed as a distributed system that allows for dynamic
scaling of benchmarking (compute) nodes.  Under this design, compute nodes 
execute benchmarks and aggregate results on shared storage (NFS, with emulab),
and a webserver serves results from this same share.

###Emulab Setup
To facilitate easy emulab setup, `.ns` files have been included in the 
`emulabSetup` folder.  These `.ns` files are configuration files for emulab, and
they specify hardware selection, drive configuration, OS selection, and specify 
a startup script to run on the first OS boot.

To setup SMACKBench on emulab,
- Create a new project.
- Copy the contents of the `emulabSetup` folder into 
  `/proj/<projectName>/scripts/`.
- Create a new experiment for each `.ns` file in the scripts folder.
  - The location of the startup scripts will have to be adjusted to reflect the
    `<projectName>` portion of the `/proj/<projectName>/scripts/` path.

At this point, it should be possible to swap experiments in and begin using 
SMACKBench.  See [emulab instructions] for setting up projects and swapping in
experiments.

For instructions on using SMACKBench, see [SMACKBench usage](SMACKBenchUsage.md).


