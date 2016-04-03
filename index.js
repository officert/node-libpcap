const libpcap = require('./build/Release/addon');

console.log(libpcap.createSession()); // 'world'
