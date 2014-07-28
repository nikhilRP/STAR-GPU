nvbio_module(star)

addsources(
    star.cpp
)
nvbio_add_module_directory(cuda)

cuda_add_executable(star ${star_srcs})
target_link_libraries(star nvbio zlibstatic crcstatic ${SYSTEM_LINK_LIBRARIES})
