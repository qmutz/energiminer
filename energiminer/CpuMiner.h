/*
 * CpuMiner.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ranjeet
 */

#ifndef ENERGIMINER_CPUMINER_H_
#define ENERGIMINER_CPUMINER_H_

#include "energiminer/miner.h"

namespace energi
{
  class CpuMiner : public Miner
  {
  public:
    CpuMiner(const Plant &plant, int index);

    virtual ~CpuMiner()
    {}

  protected:
    void trun();
  };

} /* namespace energi */

#endif /* ENERGIMINER_CPUMINER_H_ */
