'use strict';

class TransportLayer {
  constructor(buffer) {
    if (!buffer) throw new Error('TransportLayer - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('TransportLayer - constructor - buffer must be an instance of Buffer');

    
  }
}

module.exports = TransportLayer;
