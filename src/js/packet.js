'use strict';

class Packet {
  constructor(buffer) {
    if (!buffer) throw new Error('Packet - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('Packet - constructor - buffer must be an instance of Buffer');

    console.log('PACKET LENGTH - BUFFER', buffer.length);

    this.destMacAddress = _formatMacAddress(buffer.slice(0, 6).toString('hex'));
    this.srcMacAddress = _formatMacAddress(buffer.slice(6, 12).toString('hex'));
  }
}

function _formatMacAddress(macAddressStr) {
  return macAddressStr
    .match(/.{1,2}/g)
    .join(':');
}

module.exports = Packet;
