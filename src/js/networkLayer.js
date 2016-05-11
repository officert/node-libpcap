'use strict';

class NetworkLayer {
  constructor(buffer) {
    if (!buffer) throw new Error('NetworkLayer - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('NetworkLayer - constructor - buffer must be an instance of Buffer');

    
  }
}

module.exports = NetworkLayer;
