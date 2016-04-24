'use strict';

const addon = require('./build/Release/addon');
const EventEmitter = require('events');

const EVENTS = {
  PACKET: 'PACKET'
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
    this._session.open(deviceName);
  }
}

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession,
  EVENTS: EVENTS
};
