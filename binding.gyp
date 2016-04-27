{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/main.cpp",
        "src/pcapSession.h",
        "src/pcapSession.cpp",
        "src/callbackInfo.h",
        "src/ipAddress.h",
        "src/ipHeader.h",
        "src/macAddress.h",
        "src/ethernetHeader.h"
        ],
       "link_settings": {
          "libraries": [
            "-lpcap"
          ]
        }
    }
  ]
}
