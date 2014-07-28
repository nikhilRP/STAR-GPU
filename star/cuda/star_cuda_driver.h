#pragma once

#include <nvbio/io/sequence/sequence.h>
#include <nvbio/io/fmindex/fmindex.h>
#include <map>
#include <string>

namespace nvbio {
namespace star {
namespace cuda {

int driver(const char*                              output_name,
           const io::SequenceData&                  reference_data,
           const io::FMIndexData&                   driver_data,
                 io::SequenceDataStream&            read_data_stream,
           const std::map<std::string,std::string>& options);

int driver(const char*                              output_name,
           const io::SequenceData&                  reference_data,
           const io::FMIndexData&                   driver_data,
           const io::PairedEndPolicy                pe_policy,
                 io::SequenceDataStream&            read_data_stream1,
                 io::SequenceDataStream&            read_data_stream2,
           const std::map<std::string,std::string>& options);

} // namespace cuda
} // namespace star
} // namespace nvbio
