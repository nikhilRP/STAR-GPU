//#define NVBIO_ENABLE_PROFILING

#define MOD_NAMESPACE
#define MOD_NAMESPACE_BEGIN namespace star { namespace driver {
#define MOD_NAMESPACE_END   }}
#define MOD_NAMESPACE_NAME star::driver

#include <nvbio/basic/cuda/arch.h>
#include <nvbio/basic/timer.h>
#include <nvbio/basic/console.h>
#include <nvbio/basic/options.h>
#include <nvbio/basic/threads.h>
#include <nvbio/basic/html.h>
#include <nvbio/basic/dna.h>
#include <nvbio/fmindex/bwt.h>
#include <nvbio/fmindex/ssa.h>
#include <nvbio/fmindex/fmindex.h>
#include <nvbio/fmindex/fmindex_device.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/scan.h>
#include <thrust/sort.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "star_cuda_driver.h"
#include "input_thread.h"

namespace nvbio {
namespace star {
namespace cuda {

int driver(
    const char*                              output_name, 
    const io::SequenceData&                  reference_data_host,
    const io::FMIndexData&                   driver_data_host,
          io::SequenceDataStream&            read_data_stream,
    const std::map<std::string,std::string>& options)
{
    log_visible(stderr, "STAR cuda driver... started\n");

    uint32 BATCH_SIZE = 500000;

    // setup the input thread
    InputThread input_thread( &read_data_stream, BATCH_SIZE );
    input_thread.create();

    uint32 input_set  = 0;
    uint32 n_reads    = 0;

    // loop through the batches of reads
    for (uint32 read_begin = 0; true; read_begin += BATCH_SIZE)
    {
        // poll until the current input set is loaded...
        while (input_thread.read_data1[ input_set ] == NULL) {}

        io::SequenceDataHost* read_data_host1 = input_thread.read_data1[ input_set ];

        if (read_data_host1 == (io::SequenceDataHost*)InputThread::INVALID)
            break;

        io::SequenceDataDevice read_data1( *read_data_host1);

        // mark this set as ready to be reused
        input_thread.read_data1[ input_set ] = NULL;

        // advance input set pointer
        input_set = (input_set + 1) % InputThread::BUFFERS;
        printf("Count - %d\n", read_data1.size());
    }
    log_visible(stderr, "STAR cuda driver... done\n");
    return 0;
}

//
// paired-end driver
//
int driver(
    const char*                              output_name, 
    const io::SequenceData&                  reference_data_host,
    const io::FMIndexData&                   driver_data_host,
    const io::PairedEndPolicy                pe_policy,
          io::SequenceDataStream&            read_data_stream1,
          io::SequenceDataStream&            read_data_stream2,
    const std::map<std::string,std::string>& options)
{
    log_visible(stderr, "STAR cuda driver... started\n");
    log_visible(stderr, "STAR cuda driver... done\n");
    return 0;
}

} // namespace cuda
} // namespace star
} // namespace nvbio
