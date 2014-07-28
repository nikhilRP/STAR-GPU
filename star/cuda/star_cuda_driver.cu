//#define NVBIO_ENABLE_PROFILING

#define MOD_NAMESPACE
#define MOD_NAMESPACE_BEGIN namespace spp { namespace driver {
#define MOD_NAMESPACE_END   }}
#define MOD_NAMESPACE_NAME spp::driver

#include "star_cuda_driver.h"
#include "input_thread.h"

namespace nvbio {
namespace star {
namespace cuda {

int driver(io::SequenceDataStream& read_data_stream1)
{
    log_visible(stderr, "STAR cuda driver... started\n");

    uint32 BATCH_SIZE = 500000;

    // setup the input thread
    InputThread input_thread( &read_data_stream1, BATCH_SIZE );
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

} // namespace cuda
} // namespace star
} // namespace nvbio
