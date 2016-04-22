'use strict';

const addon = require('./build/Release/addon');

class NodePcap {
  lookupDevice() {
    return addon.lookupDevice();
  }

  createSession(deviceName) {
    return addon.createSession(deviceName);
  }
}

module.exports = new NodePcap();
