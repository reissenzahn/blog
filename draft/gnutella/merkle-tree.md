
Usually, a cryptographic hash function such as SHA-2 is used for the hashing.If the hash tree only needs to protect against unintentional damage, unsecured checksums such as CRCs can be used.



# Introduction

Demonstrating that a leaf node is a part of a given binary hash tree require computing a number of hashes proportional to the logarithm of the number of leaf nodes of the tree; this contrasts with hash lists, where the number is proportional to the number of leaf nodes itself.

Hash trees can be used to verify any kind of data stored, handled and transferred in and between computers. They can help ensure that data blocks received from other peers in a peer-to-peer network are received undamaged and unaltered, and even to check that the other peers do not lie and send fake blocks.


# Description

Most hash tree implementations are binary (two child nodes under each node) but they can just as well use many more child nodes under each node.



# Operation

In the top of a hash tree there is a top hash (or root hash or master hash). Before downloading a file on a p2p network, in most cases the top hash is acquired from a trusted source, for instance a friend or a web site that is known to have good recommendations of files to download. When the top hash is available, the hash tree can be received from any non-trusted source, like any peer in the p2p network. Then, the received hash tree is checked against the trusted top hash, and if the hash tree is damaged or fake, another hash tree from another source will be tried until the program finds one that matches the top hash.

The main difference from a hash list is that one branch of the hash tree can be downloaded at a time and the integrity of each branch can be checked immediately, even though the whole tree is not available yet. For example, in the picture, the integrity of data block L2 can be verified immediately if the tree already contains hash 0-0 and hash 1 by hashing the data block and iteratively combining the result with hash 0-0 and then hash 1 and finally comparing the result with the top hash. Similarly, the integrity of data block L3 can be verified if the tree already has hash 1-1 and hash 0. This can be an advantage since it is efficient to split files up in very small data blocks so that only small blocks have to be re-downloaded if they get damaged. If the hashed file is very big, such a hash tree or hash list becomes fairly big. But if it is a tree, one small branch can be downloaded quickly, the integrity of the branch can be checked, and then the downloading of data blocks can start.




# Benefits

However, it is time-consuming and computationally expensive to check the entirety of each file whenever a system wants to verify data. So, this is why Merkle trees are used. Basically, we want to limit the amount of data being sent over a network (like the Internet) as much as possible. So, instead of sending an entire file over the network, we just send a hash of the file to see if it matches. The protocol goes like this:

Computer A sends a hash of the file to computer B.
Computer B checks that hash against the root of the Merkle tree.
If there is no difference, we're done! Otherwise, go to step 4.
If there is a difference in a single hash, computer B will request the roots of the two subtrees of that hash.
Computer A creates the necessary hashes and sends them back to computer B.
Repeat steps 4 and 5 until you've found the data blocks(s) that are inconsistent. It's possible to find more than one data block that is wrong because there might be more than one error in the data.

Note that each time a hash is found to match, we need n more comparisons at the next level, where n is the branching factor of the tree.

Because the computers are only sending hashes over the network (not the entire file), this process can go very quickly. Plus, if an inconsistent piece of data is found, it's much easier to insert a small chunk of fixed data than to completely rewrite the entire file to fix the issue.




# Inclusion Proofs

We only need to provide the data we're proving exists, the Merkle root, and sibling hashes along the path from the leaf up to the root. This should require only O(log n) hashes to transmit over the wire. If you redo all of the hashing and the roots match, you will know with certainty that quotation was indeed part of the e-book. This kind of proof is known as a Merkle proof.



# Tiger Tree

The Tiger tree hash is a widely used form of hash tree. It uses a binary hash tree, usually hash a block size of 1024 bytes and uses the Tiger hash.




# Resources

- https://en.wikipedia.org/wiki/Merkle_tree

- https://web.archive.org/web/20080316033726/http://www.open-content.net/specs/draft-jchapweske-thex-02.html
- https://en.wikipedia.org/wiki/Tiger_(hash_function)

- https://www.aosabook.org/en/nosql.html

- https://github.com/keybase/go-merkle-tree
- https://github.com/xsleonard/go-merkle
- https://gitlab.com/NebulousLabs/merkletree/-/tree/master/
- https://github.com/cbergoon/merkletree
