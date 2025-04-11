This repo contains a local IP generated with https://github.com/Digilent/axi4lite_ip_gen. To rebuild it:
- `clone https://github.com/Digilent/axi4lite_ip_gen -b dev/artvvb`
- wipe the contents of axi4lite_ip_gen/specifications
- copy the contents of this repo's repo/spec folder into axi4lite_ip_gen/specifications
- wipe the contents of .../hw/repo/local
- in axi4lite_ip_gen, run `sh make_all.sh ./ip_repo`
- copy axi4lite_ip_gen/ip_repo contents to .../hw/repo/local