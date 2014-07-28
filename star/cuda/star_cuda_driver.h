#pragma once

#include <nvbio/io/sequence/sequence.h>
#include <nvbio/io/fmindex/fmindex.h>
#include <map>
#include <string>

namespace nvbio {
namespace star {
namespace cuda {

int driver(io::SequenceDataStream& read_data_stream1);

} // namespace cuda
} // namespace star
} // namespace nvbio
