{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/main.cpp",
        "src/pcap_session.h",
        "src/pcap_session.cpp"
        ],
       "link_settings": {
          "libraries": [
            "-lpcap"
          ]
        }
    }
  ]
}
