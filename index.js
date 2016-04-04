'use strict';

const addon = require('./build/Release/addon');

const DEVICE_NAME = 'en0';

let session = addon.createSession(DEVICE_NAME);

console.log(session);
// console.log(session.deviceName);
