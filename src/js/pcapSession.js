'use strict';

const addon = require('../../build/Release/addon');
const EventEmitter = require('events');

const Packet = require('./packet');

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

  /**
   * @static
   * @desc enum of events emitted by PcapSession
   */
  static get EVENTS() {
    return EVENTS;
  }

  /**
   * @desc open the PcapSession to start packet capture
   * @param {String} deviceName - device interface name
   @ @return PcapSession instance
   */
  open(deviceName) {
    this._session.open(deviceName, {
      packetCount: this._packetCount
    }, buffer => {
      let packet = new Packet(buffer);

      this.emit(EVENTS.PACKET_RECIEVED, packet);
    });

    return this;
  }

  /**
   * @desc configure the number of packets to capture
   * @param {Number} count - number of packets to capture
   @ @return PcapSession instance
   */
  packetCount(count) {
    if (!count) throw new Error('PcapSession - packetCount - count is required');

    this._packetCount = count;

    return this;
  }
}

module.exports = PcapSession;
