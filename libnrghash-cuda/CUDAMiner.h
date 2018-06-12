/*
This file is part of cpp-ethereum.

cpp-ethereum is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cpp-ethereum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "ethash_cuda_miner_kernel.h"

#include "nrgcore/plant.h"
#include "nrgcore/miner.h"

#include <cstdint>
#include <mutex>
#include <tuple>

#include <functional>

namespace energi {

class CUDAMiner: public Miner
{

public:
	CUDAMiner(const Plant& plant, unsigned _index);
	~CUDAMiner() override;

	static unsigned instances()
	{
		return s_numInstances > 0 ? s_numInstances : 1;
	}
	static unsigned getNumDevices();
	static void listDevices();
	static void setParallelHash(unsigned _parallelHash);
	static bool configureGPU(
		unsigned _blockSize,
		unsigned _gridSize,
		unsigned _numStreams,
		unsigned _scheduleFlag,
		unsigned _dagLoadMode,
		unsigned _dagCreateDevice,
		bool _noeval,
		bool _exit
		);
	static void setNumInstances(unsigned _instances);
	static void setDevices(const std::vector<unsigned>& _devices, unsigned _selectedDeviceCount);
	static bool cuda_configureGPU(
		size_t numDevices,
		const std::vector<int>& _devices,
		unsigned _blockSize,
		unsigned _gridSize,
		unsigned _numStreams,
		unsigned _scheduleFlag,
		bool _noeval
		);

	bool cuda_init(
		size_t numDevices,
		uint32_t height,
		unsigned _deviceId,
		bool _cpyToHost,
		uint8_t * &hostDAG,
		unsigned dagCreateDevice);

	void search(
		uint8_t const* header,
		uint64_t target,
		uint64_t startNonce,
		Work& w)

	/* -- default values -- */
	/// Default value of the block size. Also known as workgroup size.
	static unsigned const c_defaultBlockSize;
	/// Default value of the grid size
	static unsigned const c_defaultGridSize;
	// default number of CUDA streams
	static unsigned const c_defaultNumStreams;

protected:
	void onSetWork() override;

private:
    std::atomic<bool> m_new_work = {false};

    void trun() override;

    bool init_dag(uint32_t height);

    bool     m_dagLoaded = false;

    ///Constants on GPU
    hash128_t* m_dag = nullptr;
    std::vector<hash64_t*> m_light;
    uint32_t m_dag_size = 0;
    uint32_t m_device_num;
    uint64_t m_lastHeight;

    volatile search_results** m_search_buf;
    cudaStream_t* m_streams;
	uint64_t m_current_target;

    /// The local work size for the search
    static unsigned s_blockSize;
    /// The initial global work size for the searches
    static unsigned s_gridSize;
    /// The number of CUDA streams
    static unsigned s_numStreams;
    /// CUDA schedule flag
    static unsigned s_scheduleFlag;

    static unsigned m_parallelHash;

    static unsigned s_numInstances;
    static std::vector<int> s_devices;

    static bool s_noeval;

};

} //namespace energi
