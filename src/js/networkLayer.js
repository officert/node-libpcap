'use strict';

const TransportLayer = require('./transportLayer');

class NetworkLayer {
  constructor(buffer) {
    if (!buffer) throw new Error('NetworkLayer - constructor - buffer is required');
    if (!(buffer instanceof Buffer)) throw new Error('NetworkLayer - constructor - buffer must be an instance of Buffer');

    //ip packet diagram : https://josephmlod.files.wordpress.com/2010/12/ip-header-diagram.jpg

    //TODO: the parsing here might depend on the etherType at the link layer (IPv4, IPv6, ARP etc...)

    this.version = (buffer[0] & 0xf0) >> 4; //jshint ignore:line
    this.headerLength = (buffer[0] & 0x0f) << 2; //jshint ignore:line
    this.typeOfService = buffer.slice(1, 2).toString('hex');
    this.totalLength = buffer.slice(2, 4).toString('hex');
    this.identification = buffer.slice(4, 6).toString('hex');

    //TODO:
    // this.flags
    // this.fragmentOffset

    this.timeToLive = buffer.slice(8, 9).toString('hex');
    this.protocol = buffer.slice(9, 10).toString('hex');
    this.headChecksum = buffer.slice(10, 12).toString('hex');

    this.srcIpAddress = _hexToIpAddress(buffer.slice(12, 16).toString('hex'));
    this.destIpAddress = _hexToIpAddress(buffer.slice(16, 20).toString('hex'));

    this.transportLayer = new TransportLayer(buffer); //TODO: slice this buffer and pass a new buffer that consists of only transport layer bytes
  }
}

function _hexToIpAddress(hex) {
  let ip = '';

  for (let i = 0; i < hex.length; i += 2) {
    let sub = hex.substring(i, i + 2);
    let num = parseInt(sub, 16);
    ip += num + '.';
  }

  //remove the last .
  ip = ip.substring(0, ip.length - 1);

  return ip;
}

module.exports = NetworkLayer;
