# YCSB-C

Yahoo! Cloud Serving Benchmark in C++, a C++ version of YCSB (https://github.com/brianfrankcooper/YCSB/wiki) with LevelDB and NoveLSM

## Quick Start

To build YCSB-C on Ubuntu, for example:

```
$ git clone --recurse-submodules https://github.com/Guslarz/YCSB-C.git
$ sudo apt-get install libtbb-dev
$ make
```

Run Workload A with a [TBB](https://www.threadingbuildingblocks.org)-based
implementation of the database, for example:
```
./ycsbc -db tbb_rand -threads 4 -P workloads/workloada.spec
```
Also reference run.sh and for the command line. See help by
invoking `./ycsbc` without any arguments.

Note that we do not have load and run commands as the original YCSB. Specify
how many records to load by the recordcount property. Reference properties
files in the workloads dir.

## Persistent-team

Run `./persistent-test.sh` to see that build works.

## LevelDB specific

#### Requirements

- Boost - `sudo apt install libboost-all-dev` on Ubuntu
- LevelDB - [https://github.com/google/leveldb](https://github.com/google/leveldb)

#### How to run

Additional `-dbpath` argument required e.g.

`./ycsbc -db leveldb -dbpath ../database -threads 1 -P workloads/workloada.spec`

## NoveLSM

#### Requirements

- Boost
- NoveLSM - from [here](https://github.com/Guslarz/lsm_nvm), not original repo (it duplicates leveldb namespace)

#### How to run

Additional `-dbdisk` and `-dbmem` argument required e.g.

`./ycsbc -db leveldb -threads 1 -P workloads/workloada.spec -dbdisk /mnt/mem -dbmem /mnt/mem`

#### TODO

- find out the difference between dbdisk and dbmem
