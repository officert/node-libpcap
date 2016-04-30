'use strict';

const addon = require('./build/Release/addon');
const EventEmitter = require('events');

const Packet = require('./packet');

const EVENTS = {
  PACKET_RECIEVED: 'PACKET_RECIEVED'
};

const DEFAULT_PACKET_COUNT = 10;

function _lookupDevice() {
  return addon.lookupDevice();
}

class PcapSession extends EventEmitter {
  constructor() {
    super();

    this._session = new addon.PcapSession();
    this._packetCount = DEFAULT_PACKET_COUNT;
  }

  open(deviceName) {
    this._session.open(deviceName, {
      packetCount: this._packetCount
    }, packet => {
      this.emit(EVENTS.PACKET_RECIEVED, new Packet(packet));
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

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession,
  EVENTS: EVENTS
};
