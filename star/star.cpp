/******************************************************************************
  star.cpp

  (c) 2014 - Nikhil R Podduturi
  Cherry Lab, Stanford University

  Licensed under the GNU General Public License 2.0 license.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nvbio/basic/timer.h>
#include <nvbio/basic/console.h>
#include <nvbio/io/sequence/sequence.h>
#include <nvbio/basic/shared_pointer.h>
#include <nvbio/io/fmindex/fmindex.h>

#include "cuda/star_cuda_driver.h"

void crcInit();

namespace nvbio {
namespace star {
namespace cuda {


} // namespace cuda
} // namespace star
} // namespace nvbio

using namespace nvbio;

int main(int argc, char* argv[])
{
    cudaSetDeviceFlags( cudaDeviceMapHost | cudaDeviceLmemResizeToMax );

    crcInit();
    if (argc == 1 ||
        (argc == 2 && strcmp( argv[1], "--help" ) == 0) ||
        (argc == 2 && strcmp( argv[1], "-h" ) == 0))
    {
        log_info(stderr,"star [options] reference-genome read-file output\n");
        exit(0);
    }

    //bool   debug      = false;
    int cuda_device  = -1;
    int device_count;
    cudaGetDeviceCount(&device_count);
    log_verbose(stderr, "  cuda devices : %d\n", device_count);

    // inspect and select cuda devices
    if (device_count)
    {
        if (cuda_device == -1)
        {
            int            best_device = 0;
            cudaDeviceProp best_device_prop;
            cudaGetDeviceProperties( &best_device_prop, best_device );
            for (int device = 0; device < device_count; ++device)
            {
                cudaDeviceProp device_prop;
                cudaGetDeviceProperties( &device_prop, device );
                if (device_prop.major >= best_device_prop.major &&
                    device_prop.minor >= best_device_prop.minor)
                {
                    best_device_prop = device_prop;
                    best_device      = device;
                }
            }
            cuda_device = best_device;
        }
        log_verbose(stderr, "  chosen device %d\n", cuda_device);
        {
            cudaDeviceProp device_prop;
            cudaGetDeviceProperties( &device_prop, cuda_device );
            log_verbose(stderr, "    device name        : %s\n", device_prop.name);
            log_verbose(stderr, "    compute capability : %d.%d\n", device_prop.major, device_prop.minor);
        }
        cudaSetDevice( cuda_device );
    }

    io::QualityEncoding qencoding = io::Phred33;
    int arg = 1;
    try
    {
        log_visible(stderr, "loading reference index... started\n");
        log_info(stderr, "  file: \"%s\"\n", argv[arg]);

        // load the reference data
        reference_data = io::load_sequence_file( DNA, argv[arg] );
        if (reference_data == NULL)
        {
            log_error(stderr, "unable to load reference index \"%s\"\n", argv[arg]);
            return 1;
        }

        log_visible(stderr, "loading reference index... done\n");
        nvbio::io::FMIndexDataHost* loader = new nvbio::io::FMIndexDataHost;
        if (!loader->load( argv[arg] ))
            return 1;
        driver_data = loader;

        const char *read_file_name= argv[arg+1];

        log_visible(stderr, "opening read file [1] \"%s\"\n", read_file_name);
        SharedPointer<nvbio::io::SequenceDataStream> read_data_file(
            nvbio::io::open_sequence_file(read_file_name,
                qencoding,
                uint32(-1),
                uint32(-1),
                io::REVERSE)
            );

        if (read_data_file == NULL || read_data_file->is_ok() == false)
        {
            log_error(stderr, "unable to open read file \"%s\"\n", read_file_name);
            return 1;
        }

        nvbio::star::cuda::driver( *read_data_file);
    }
    catch (nvbio::cuda_error &e)
    {
        log_error(stderr, "caught a nvbio::cuda_error exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (nvbio::bad_alloc &e)
    {
        log_error(stderr, "caught a nvbio::bad_alloc exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (nvbio::logic_error &e)
    {
        log_error(stderr, "caught a nvbio::logic_error exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (nvbio::runtime_error &e)
    {
        log_error(stderr, "caught a nvbio::runtime_error exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (std::bad_alloc &e)
    {
        log_error(stderr, "caught a std::bad_alloc exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (std::logic_error &e)
    {
        log_error(stderr, "caught a std::logic_error exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (std::runtime_error &e)
    {
        log_error(stderr, "caught a std::runtime_error exception:\n");
        log_error(stderr, "  %s\n", e.what());
    }
    catch (...)
    {
        log_error(stderr, "caught an unknown exception!\n");
    }

    return 0;
}
