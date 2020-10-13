
https://en.wikipedia.org/wiki/Conflict-free_replicated_data_type

https://lars.hupel.info/topics/crdt/01-intro/

https://hal.inria.fr/file/index/docid/555588/filename/techreport.pdf

https://github.com/neurodrone/crdt


# State-Based Increment-Only Counter (G-Counter)

Suppose the payload was a single integer and merge computes max. This data type is a CvRDT as its states form a monotonic semi-lattice. Consider two replicas, with the same initial state of 0; at each one, a client originates increment. They converge to 1 instead of the expected 2.

Suppose instead the payload is an integer and merge adds the two values. This is not a CvRDT, as merge is not idempotent.





