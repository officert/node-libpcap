'use strict';

const nodePcap = require('../index');

let deviceName = nodePcap.lookupDevice();

if(!deviceName) throw new Error('Error looking up device name');

let session = nodePcap.createSession(deviceName);

console.log('DEVICE NAME', nodePcap.lookupDevice());
console.log('SESSION', session);
