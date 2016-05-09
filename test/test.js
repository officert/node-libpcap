'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if (!deviceName) throw new Error('Error looking up device name');

console.log('deviceName', deviceName);

let session = new nodePcap.PcapSession();

console.log('session', session);

session.on(nodePcap.PcapSession.EVENTS.PACKET_RECIEVED, packet => {
  console.log('packet', packet);
});

session
  .packetCount(30)
  .open(deviceName);
