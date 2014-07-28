#include <nvbio/io/alignments.h>
#include <nvbio/io/output/output_utils.h>
#include <nvbio/basic/threads.h>
#include <nvbio/basic/timer.h>

#include "input_thread.h"

namespace nvbio {
namespace star {
namespace cuda {

void InputThread::run()
{
    log_verbose( stderr, "starting background input thread\n" );

    while (1u)
    {
        // poll until the set is done reading & ready to be reused
        while (read_data1[m_set] != NULL) {}

        Timer timer;
        timer.start();

        const int ret1 = io::next( DNA_N, &read_data_storage1[ m_set ], m_read_data_stream1, m_batch_size );

        timer.stop();

        if (ret1)
        {
            // mark the set as done
            read_data1[ m_set ] = &read_data_storage1[ m_set ];
        }
        else
        {
            // mark this as an invalid entry
            read_data1[ m_set ] = (io::SequenceDataHost*)INVALID;
            break;
        }

        // switch to the next set
        m_set = (m_set + 1) % BUFFERS;
    }
}

} // namespace cuda
} // namespace star
} // namespace nvbio
