'use strict';

/**
 * @class Packet
 */
class Packet {
  /**
   * @param {Object} options
   */
  constructor(options) {
    options = options || {};

    this.deviceName = options.deviceName;
    this.destMacAddress = options.destMacAddress;
    this.srcMacAddress = options.srcMacAddress;
  }
}

module.exports = Packet;
