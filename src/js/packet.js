'use strict';

const LinkLayer = require('./linkLayer');

class Packet {
  constructor(buffer) {
    if (!buffer) throw new Error('Packet - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('Packet - constructor - buffer must be an instance of Buffer');

    this.linkLayer = new LinkLayer(buffer);
  }
}

module.exports = Packet;
