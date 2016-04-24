'use strict';

const addon = require('./build/Release/addon');

function _lookupDevice() {
  return addon.lookupDevice();
}

class PcapSession {
  constructor() {
    this._session = new addon.PcapSession();
  }

  open(deviceName) {
    this._session.open(deviceName);
  }
}

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession
};
