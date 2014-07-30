STAR-GPU
========

CUDA implementation of STAR aligner.

* This almost uses 75% code base from nvBowtie (GPU version of Bowtie2 aligner which is part of nvbio).

Changes from nvBowtie:

* Seeding (Maximum mapability length), in other words exact match of max substring of the read.
* Scoring is done by Sharon Entropy principle
* Splice junction calculation? (for later stages)
