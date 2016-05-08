'use strict';

const addon = require('./build/Release/addon');

const PcapSession = require('./src/js/pcapSession');

function _lookupDevice() {
  return addon.lookupDevice();
}

module.exports = {
  lookupDevice: _lookupDevice,
  PcapSession: PcapSession
};
