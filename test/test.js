'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if (!deviceName) throw new Error('Error looking up device name');

console.log('deviceName', deviceName);

let session = new nodePcap.PcapSession();

console.log('session', session);

session.on(nodePcap.PcapSession.EVENTS.PACKET_RECIEVED, buffer => {
  // console.log(buffer);
});

session
  .packetCount(30)
  .open(deviceName);
