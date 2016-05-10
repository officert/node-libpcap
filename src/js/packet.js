'use strict';

const ETHER_TYPES = require('./enums/etherTypes');

class Packet {
  constructor(buffer) {
    if (!buffer) throw new Error('Packet - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('Packet - constructor - buffer must be an instance of Buffer');

    let offset = 0;

    this.destMacAddress = _formatMacAddress(buffer.slice(offset, offset + 6).toString('hex'));

    offset += 6;

    this.srcMacAddress = _formatMacAddress(buffer.slice(offset, offset + 6).toString('hex'));

    offset += 6;

    this.etherType = _getEtherType(buffer, offset);
  }
}

function _formatMacAddress(macAddressStr) {
  return macAddressStr
    .match(/.{1,2}/g)
    .join(':');
}

function _getEtherType(buffer, offset) {
  //read 16 bits (2 bytes, which is the size of the etherType)
  //in big endian order
  let strType = buffer.readUInt16BE(offset, true);

  let type = null;

  switch (strType) {
    case 0x0800:
      type = ETHER_TYPES.IPV4;
      break;
    case 0x8100:
      type = ETHER_TYPES.IPV6;
      break;
    case 0x0806:
      type = ETHER_TYPES.ARP;
      break;
    default:
  }

  return type;
}

module.exports = Packet;
