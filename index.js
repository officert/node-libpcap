'use strict';

const addon = require('./build/Release/addon');
const EventEmitter = require('events');

const EVENTS = {
  PACKET_RECIEVED: 'PACKET_RECIEVED'
};

function _lookupDevice() {
  return addon.lookupDevice();
}

class PcapSession extends EventEmitter {
  constructor() {
    super();

    this._session = new addon.PcapSession();
  }

  open(deviceName) {
    this._session.open(deviceName, (packet) => {
      if (packet) {
        this.emit(EVENTS.PACKET_RECIEVED, packet);
      }
    });
  }
}

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession,
  EVENTS: EVENTS
};
