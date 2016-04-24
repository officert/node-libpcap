'use strict';

const addon = require('./build/Release/addon');

function _lookupDevice() {
  return addon.lookupDevice();
}

class PcapSession {
  constructor() {
    this._session = new addon.PcapSession();
  }

  open() {
    this._session.open();
  }
}

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession
};
