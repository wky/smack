#SMACKBench Installation

###A Note on Hardware

SMACKBench was originally designed for use on any machine, however, most of the
recent effort has gone towards making SMACKBench work well in a distributed
[emulab](http://emulab.net/) environment.  As a result, the scripted
installation procedures focus on setup within this environment, and the single
server installation scripts are not kept up to date.

With that said, a small section can be found near the end of this document with
guidelines for patching together a SMACKBench installation on a single machine.

##Emulab Installation
Emulab is a shared testbed infrastructure available to researchers.  As such,
hardware must be shared by research teams.  This means experiments get swapped
out after a certain amount of time.  A consequence of this is that prerequisites
must be reinstalled every time a node gets swapped in.  To automate this,
build scripts are available for each type of node.  These will install 
prerequisites on both compute nodes, and the webserver node.  The SMACKBench
installation itself installs to shared storage, and only has to be done once.

To facilitate easy emulab setup, `.ns` files have been included in the 
`svcomp/emulabSetup` folder.  These `.ns` files are configuration files for 
emulab, and they specify hardware selection, drive configuration, OS selection,
and specify a startup script to run on the first OS boot.

To setup SMACKBench on emulab,
- Create a new project.
- From the shared storage file server, clone SMACK to 
  `/proj/<projectName>/smack`.
- Copy the contents of the `/proj/<projectName>/smack/svcomp/emulabSetup` folder
  into a new scripts folder: `/proj/<projectName>/scripts/`.
- Create a new experiment for each `.ns` file in the `scripts` folder.
  - The location of the startup scripts will have to be adjusted in the `.ns` 
    files to reflect the `<projectName>` portion of the 
    `/proj/<projectName>/scripts/` path.
- Modify the `/proj/<projectName>/scripts/smackbench_webserver_buildscript.sh`
  to reflect the new `<projectName>` portion of the apache document root path.
- On emulab, swap in the webserver, and one of the compute nodes. Wait for the
  compute node to reboot before proceeding.
- From the swapped in compute node, navigate to 
  `/proj/<projectName>/smack/svcomp/bench/` and execute
  `./setupSMACKBench.sh /proj/<projectName>/SMACKBenchResults/`

At this point, it should be possible to swap in compute nodes and begin using
SMACKBench.  See [emulab instructions](https://wiki.emulab.net/Emulab/wiki) for
setting up projects and swapping in experiments.

For instructions on using SMACKBench, see [SMACKBench usage](SMACKBenchUsage.md).

##Generic Distributed Installation
This has not been tested or used, but assuming there is shared storage
available from all hosts in your environment, it shouldn't be too difficult to
get things working in a custom environment.  Further, documenting this helps in
demonstrating the design of SMACKBench for troubleshooting.

To set up SMACKBench in a non-emulab environment (adjust shared storage paths 
as needed):
- Deploy Ubuntu 14.04 LTS 64-bit to each compute node and the webserver node.
- From the shared storage file server, clone SMACK to 
  `/<shareName>/smack`, where `<shareName>` is the name of a shared storage
  share available from all nodes.
- Copy the contents of the `/<shareName>/smack/svcomp/emulabSetup` folder
  into a new scripts folder: `/<shareName>/scripts/`.
- Create a scripts folder on a shared storage share accessible from all nodes.
- Copy the `svcomp/emulabSetup` contents to the new scripts folder.
- On first OS boot, execute the appropriate buildscript (see 
  [SMACKBench Usage](SMACKBenchUsage.md)) on each compute node and 
  `smackbench_webserver_buildscript.sh` on the webserver node.  These can be 
  found in the `/<shareName>/scripts/` folder.  Wait for compute nodes to 
  reboot.
  - The webserver buildscript will need to be adjusted to account for the
    change in the shared folder name/path.
- From one of the compute nodes, navigate to `/<shareName>/smack/svcomp/bench/`
   and execute `./setupSMACKBench.sh /<shareName>/SMACKBenchResults/`

At the point, SMACKBench should be ready for use.  For instructions on using
SMACKBench, see [SMACKBench usage](SMACKBenchUsage.md).


##Single Server Installation
This method of installation has also not been tried or tested, but should work.

To install SMACKBench to a single server:
- Deploy Ubuntu 14.04 LTS 64-bit.
- Clone SMACK to `<installRoot>/smack`.
- Copy the contents of `<installRoot>/smack/svcomp/emulabSetup/` to 
  `<installRoot>/scripts/`.
- Execute `<installRoot>/scripts/smackbench_compute_repo_buildscript.sh`
- Execute `<installRoot>/scripts/smackbench_webserver_buildscript.sh`
  - This script will need to be modified to reflect the correct `<installRoot>`
    path for the apache document root folder.
- From one of the compute nodes, navigate to 
  `<installRoot>/smack/svcomp/bench/` and execute
  `./setupSMACKBench.sh <installRoot>/SMACKBenchResults/`

At the point, SMACKBench should be ready for use.  For instructions on using
SMACKBench, see [SMACKBench usage](SMACKBenchUsage.md).