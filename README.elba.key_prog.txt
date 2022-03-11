1. nic_util and nic_arm must be present on NIC /data

2. Program efuse before key programming
# Initialize all NICs
cd /home/diag/diag/python/regression/
./nic_test.py -setup_multi -slot_list <>
# Iterate over all NICs and program efuse
cd /home/diag/diag/python/esec/
./esec_ctrl.py -efuse_prog -sn FLM2049000F -slot 5 -brd_name "ORTANO2" -pn "P26966-B21" -mac "00:AE:CD:01:8E:58"

# Dry run option: add "-d"
./esec_ctrl.py -efuse_prog -sn FLM2049000F -slot 5 -brd_name "ORTANO2" -pn "P26966-B21" -mac "00:AE:CD:01:8E:58" -d

3. Following the same way before to do key program with one change.
# add "-fast" to key programming.
e.g.
./esec_ctrl.py -esec_prog -sn FLM2049000F -slot 5 -brd_name "ORTANO" -pn "P26966-B21" -mac "00:AE:CD:01:8E:58" -fast

Remarks:
Default value of parameters are added. Full command format would be following:
./esec_ctrl.py -esec_prog -slot $slot -sn FLM2046000E -pn "68-0003-02 01" -mac 00:AE:CD:00:00:00 -brd_name NAPLES25SWM833 -mtp MTP100 -k certs/client.key.pem -c certs/client-bundle.cert.pem  -t certs/rootca.cert.pem -b pki1:12266#pki2:12266 -fast


