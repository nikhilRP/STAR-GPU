STAR-GPU (Still in Development)
========

CUDA implementation of STAR aligner.

* This almost uses 75% code base from nvBowtie (GPU version of Bowtie2 aligner which is part of nvbio).

Changes from nvBowtie:

* Spliced alignments are the biggest challenege and the difference.
* Seeding (Maximum mapability length), in other words exact match of biggest substring of the read.
* Scoring is done by Sharon Entropy principle.
