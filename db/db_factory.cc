//
//  basic_db.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "db/db_factory.h"

#include <string>
#include "db/basic_db.h"
#include "db/lock_stl_db.h"
#include "db/tbb_rand_db.h"
#include "db/tbb_scan_db.h"
#include "db/leveldb.h"
#include "db/leveldb_serialized.h"
#include "db/novelsm.h"
#include "db/novelsm_serialized.h"

using namespace std;
using ycsbc::DB;
using ycsbc::DBFactory;

DB* DBFactory::CreateDB(utils::Properties &props) {
  if (props["dbname"] == "basic") {
    return new BasicDB;
  } else if (props["dbname"] == "lock_stl") {
    return new LockStlDB;
  } else if (props["dbname"] == "tbb_rand") {
    return new TbbRandDB;
  } else if (props["dbname"] == "tbb_scan") {
    return new TbbScanDB;
  } else if (props["dbname"] == "leveldb") {
    if (props["serialize"] == "y")
      return new LevelDBSerialized(props["dbpath"]);
    else return new LevelDB(props["dbpath"]);
  } else if (props["dbname"] == "novelsm") {
    if (props["serialize"] == "y")
      return new NoveLSMSerialized(props["dbdisk"], props["dbmem"]);
    else return new NoveLSM(props["dbdisk"], props["dbmem"]);
  } else return nullptr;
}

