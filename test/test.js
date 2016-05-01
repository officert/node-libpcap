'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if (!deviceName) throw new Error('Error looking up device name');

let session = new nodePcap.PcapSession();

session.on(nodePcap.EVENTS.PACKET_RECIEVED, packet => {
  console.log(packet);
});

session
  .packetCount(30)
  .open(deviceName);
