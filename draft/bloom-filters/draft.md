
# Introduction

False positive matches are possible, but false negatives are not – in other words, a query returns either "possibly in set" or "definitely not in set". Elements can be added to the set, but not removed (though this can be addressed with the counting Bloom filter variant); the more items added, the larger the probability of false positives.

More generally, fewer than 10 bits per element are required for a 1% false positive probability, independent of the size or number of elements in the set.

---

Two key properties of Bloom filters:

- No false negatives: This states that if a query for an URL in the Bloom filter returns negative, then the queried item can be guaranteed to not be present in the set of malicious URLs.

- Low false positive rate: This property states that for any URL that is not in the set of malicious URLs, the likelihood of a Bloom filter query returning a positive result should be fairly low.

---

A Bloom filter is a data structure that is used to check for membership of an element x in a set of m elements.



# Properties

Bloom filters have a string space advantage to other data structures like sets, hash tables or binary search trees.

Bloom filters also have the property that the time taken to add an item or to check fo membership is constant O(k) and is independent of the number of items in the filter.


# Composition

A Bloom filter bf is a combination of two separate components:

- A bit-vector of n bits
- and a sequence of k hash functions.

---

An empty Bloom filter is a bit array of m bits all set to 0. There must also be k different hash functions defined, each of which maps or hashes some set element to one of the m array positions, generating a uniform random distribution. Typically, k is a small constant which depends on the desired false error rate \epsilon while m is proportional to k and the number of elements to be added.




# Operations

Insertion: To insert a value x into a Bloom filter bf, simply hash it over each one of the k hash function and treating the hash outputs as indices into the bit-vector, raise the corresponding bits.

Query: To query for value y in the Bloom filter, once again hash it over each one of the k hash functions and check whether all of the corresponding bits are raised.

These operations are entirely sufficient to ensure the No false negatives property.

---

To add an element, feed it to each of the k hash functions to get k array positions. Set the bits at all these positions to 1.




# False Negatives

When an item is inserted into the Bloom filter, it will result in a certain subset of the bits in the bit-vector being raised. As there are no operations that can unset these bits, we can be certain that any subsequent query for the inserted element must return true. In other words, if we get a negative result for a query, we can be certain that the item has never been inserted into the Bloom filter before.


# False Positions

This is the case for negative results; things get slightly more interesting for positive results due to the possibility of false positives.




# TODO

Typically, \(m\) is a small constant which depends on the desired false error rate \(\epsilon\), while \(m\) i proportional to \(k\) and the number of elements to be added.


