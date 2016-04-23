'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if(!deviceName) throw new Error('Error looking up device name');

console.log('DEVICE NAME', nodePcap.lookupDevice());

let session = new nodePcap.PcapSession();

console.log('SESSION', session);
