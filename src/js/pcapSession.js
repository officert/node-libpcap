'use strict';

const addon = require('../../build/Release/addon');
const EventEmitter = require('events');

const DEFAULT_PACKET_COUNT = 10;

const EVENTS = {
  PACKET_RECIEVED: 'PACKET_RECIEVED'
};

class PcapSession extends EventEmitter {
  constructor() {
    super();

    this._session = new addon.PcapSession();
    this._packetCount = DEFAULT_PACKET_COUNT;
  }

  static get EVENTS() {
    return EVENTS;
  }

  open(deviceName) {
    this._session.open(deviceName, {
      packetCount: this._packetCount
    }, buffer => {
      console.log('PACKET LENGTH - BUFFER', buffer.length);
      
      this.emit(EVENTS.PACKET_RECIEVED, buffer);
    });

    return this;
  }

  /**
   * @param {Number} count - number of packets to capture
   */
  packetCount(count) {
    if (!count) throw new Error('PcapSession - packetCount - count is required');

    this._packetCount = count;

    return this;
  }
}

module.exports = PcapSession;
