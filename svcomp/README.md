#SMACKBench
## A Tool for Benchmarking SMACK

SMACKBench is framework that configures SMACK for execution using 
[benchexec](https://github.com/sosy-lab/benchexec).

##Description
SMACKBench has been designed as a distributed system that allows for dynamic
scaling of benchmarking (compute) nodes.  Under this design, compute nodes 
execute benchmarks and aggregate results on shared storage (NFS, with emulab),
and a webserver serves results from this same share.

##Documentation
- [Installation](doc/SMACKBenchInstallation.md)
- [Usage](doc/SMACKBenchUsage.md)
