{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/main.cpp",
        "src/pcapSession.h",
        "src/pcapSession.cpp",
        "src/structs.h"
        ],
       "link_settings": {
          "libraries": [
            "-lpcap"
          ]
        }
    }
  ]
}
