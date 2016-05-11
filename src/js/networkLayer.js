'use strict';

const TransportLayer = require('./transportLayer');

class NetworkLayer {
  constructor(buffer) {
    if (!buffer) throw new Error('NetworkLayer - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('NetworkLayer - constructor - buffer must be an instance of Buffer');

    //TODO: the parsing here might depend on the etherType at the link layer (IPv4, IPv6, ARP etc...)

    // this.version = buffer.slice(0, 1).toString('hex');
    this.version = buffer.readUInt16BE(0, true);

    this.transportLayer = new TransportLayer(buffer); //TODO: slice this buffer and pass a new buffer that consists of only transport layer bytes
  }
}

module.exports = NetworkLayer;
