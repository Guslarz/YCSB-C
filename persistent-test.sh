#!/bin/bash

PMEM_NUM=0
NUMACTL="numactl -N$PMEM_NUM -m$PMEM_NUM"
PATH_COMMON="/mnt/pmem$PMEM_NUM/ycsbc-"
PATH_LEVELDB="${PATH_COMMON}leveldb"
PATH_NOVELSM="${PATH_COMMON}novelsm"
YCSBC_COMMON="./ycsbc -P workloads/workloada.spec -threads 40"
YCSBC_LEVELDB="$YCSBC_COMMON -db leveldb -dbpath $PATH_LEVELDB"
YCSBC_NOVELSM="$YCSBC_COMMON -db novelsm -dbdisk $PATH_NOVELSM -dbmem $PATH_NOVELSM"

$NUMACTL $YCSBC_LEVELDB
$NUMACTL $YCSBC_NOVELSM
