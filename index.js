'use strict';

const addon = require('./build/Release/addon');

const PcapSession = require('./src/js/pcapSession');

/**
 * @module libpcap
 */
module.exports = {
  /**
   * @desc Look up the default network interface name
   * @method lookupDevice
   * @return String
   */
  lookupDevice: () => {
    return addon.lookupDevice();
  },
  /**
   * @desc PcapSession class constructor
   * @constructor lookupDevice
   * @return PcapSession instance
   */
  PcapSession: PcapSession
};
