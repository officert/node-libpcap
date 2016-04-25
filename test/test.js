'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if (!deviceName) throw new Error('Error looking up device name');

console.log('DEVICE NAME', nodePcap.lookupDevice());

let session = new nodePcap.PcapSession();

session.on(nodePcap.EVENTS.PACKET_RECIEVED, packet => {
  console.log('PACKET RECIEVED', packet);
});

session.open(deviceName);
