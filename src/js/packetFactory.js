'use strict';

// const PACKET_TYPES = require('./enums/packetTypes');

const Packet = require('./packet');

class PacketFactory {
  /**
   * @param {Buffer} buffer - node Buffer object
   */
  create(buffer) {
    if (!buffer) throw new Error('PacketFactory - create() buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('PacketFactory - create - buffer must be an instance of Buffer');

    return new Packet(buffer);
  }
}

module.exports = new PacketFactory();
