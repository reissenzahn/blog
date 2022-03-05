const randomBytes = require('randombytes')
const { EventEmitter } = require('events')

function arrayEquals(array1, array2) {
  if (array1 === array2) return true;

  if (array1.length !== array2.length) return false;

  for (let i = 0; i < array1.length; i++) {
    if (array1[i] !== array2[i]) return false;
  }

  return true;
}

function createNode() {
  return {
    contacts: [],
    dontSplit: false,
    left: null,
    right: null,
  };
}

// contact is { id: ID }



class KBucket extends EventEmitter {

  // constructor() {
  //   super();

  //   this.localNodeID = randomBytes(20);

  //   // number of nodes that a k-bucket can contain before being full or split
  //   this.numberOfNodesPerKBucket = 20;

  //   // number of nodes to ping when a bucket that should not be split becomes full (KBucket will emit a ping event that contains numberOfNodesToPing nodes that have not been contacted the longest)
  //   this.numberOfNodesToPing = 3;
  //   this.root = createNode()
  // }

  // static distance(firstId, secondId) {
  //   let distance = 0;
    
  //   for (let i ; i < min; i++) {
  //     distance = (distance << 8) + (firstId[i] ^ secondId[i]);
  //   }
    
  //   return distance;
  // }

  // static arbiter(incumbent, candidate) {
  //   return incumbent.vectorClock > candidate.vectorClock ? incumbent : candidate;
  // }

  // add(contact) {
    // let bitIndex = 0;
    // let curr = this.root;

    // // find suitable leaf node
    // while (curr.contacts === null) {
    //   curr = this._determineNode(node, contact.id, bitIndex++)
    // }

    // check if contact already exits
    // const index = this._indexOf(node, contact.id);
    // if (index >= 0) {
    //   this._update(node, index, contact);
    //   return;
    // }

    // check if there is space to add the contact
    // if (node.contacts.length < this.numberOfNodesPerKBucket) {
    //   node.contacts.push(contact);
    //   this.emit('added', contact);
    // }

    // the bucket is full so either ping or split
    // if (node.dontSplit) {
    //   this.emit('ping', node.contacts.slice(0, this.numberOfNodesToPing), contact);
    //   return;
    // }

  //   this._split(node, bitIndex);
  //   this.add(contact);

  //   return;
  // }

  // get(id) {  
  //   let bitIndex = 0;
  //   let curr = this.root;

  //   while (curr.contacts === null) {
  //     curr = this._determineNode(curr, id, bitIndex++)
  //   }
  
  //   const index = this._indexOf(curr, id);
    
  //   return index >= 0 ? curr.contacts[index] : null;
  // }

  // remove(id) {
  //   let bitIndex = 0;
  //   let curr = this.root;

  //   while (curr.contacts === null) {
  //     curr = this._determineNode(curr, id, bitIndex++);
  //   }

  //   const index = this._indexOf(curr, id);

  //   if (index >= 0) {
  //     const contact = curr.contacts.splice(index, 1)[0];
  //     this.emit('removed', contact);
  //   }

  //   return;
  // }

  closest(id, n) {
    if (n <= 0) throw new TypeError('n is not positive number');

    let contacts = [];
    let bitIndex = 0;
    let stack = [this.root];

    while (stack.length > 0 && contacts.length < n) {
      const node = stack.pop();

      if (node.contacts === null) {
        const detNode = this._determineNode(node, id, bitIndex++);
        stack.push(node.left === detNode ? node.right : node.left);
        stack.push(detNode);
      } else {
        contacts = contacts.concat(node.contacts);
      }
    }

    return contacts
      .map(a => [this.distance(a.id, id), a])
      .sort((a, b) => a[0] - b[0])
      .slice(0, n)
      .map(a => a[1]);
  }

  toArray() {
    let result = [];
    const stack = [this.root];

    while (stack.length > 0) {
      const node = stack.pop();

      if (node.contacts === null) {
        stack.push(node.right);
        stack.push(node.left);
      } else {
        result = result.concat(node.contacts);
      }
    }

    return result;
  }

  // count() {
  //   let count = 0;
  //   const stack = [this.root];

  //   while (stack.length > 0) {
  //     const node = stack.pop();

  //     if (node.contacts === null) {
  //       stack.push(node.right);
  //       stack.push(node.left);
  //     } else {
  //       count += node.contacts.length;
  //     }
  //   }

  //   return count;
  // }

  // _determineNode(node, id, bitIndex) {
  //   if ((id[bitIndex / 8] & (1 << (7 - bitIndex % 8))) != 0) {
  //     return node.right;
  //   }

  //   return node.left;
  // }
  
  // _indexOf (node, id) {
  //   for (let i = 0; i < node.contacts.length; ++i) {
  //     if (arrayEquals(node.contacts[i].id, id)) return i;
  //   }
  
  //   return -1;
  // }

  // _split (node, bitIndex) {
  //   node.left = createNode();
  //   node.right = createNode();

  //   // redistribute existing contacts amongst children
  //   for (const contact of node.contacts) {
  //     this._determineNode(node, contact.id, bitIndex).contacts.push(contact)
  //   }

  //   // mark as inner tree node
  //   node.contacts = null;

  //   // don't split the "far away" node; we check where the local node would end up and mark the other one as "dontSplit" (i.e. "far away")
  //   const detNode = this._determineNode(node, this.localNodeId, bitIndex);
    
  //   const otherNode = node.left === detNode ? node.right : node.left;
    
  //   otherNode.dontSplit = true;
  // }
}


module.exports = KBucket;


class KBucket extends EventEmitter {



}
