# setup management port
slot=1
cd ~/diag/python/regression
./nic_test.py -setup_multi -slot_list $slot -mgmt

# IP will 10.1.1.(100+slot)
# scp nic_arm.tar nic_util.tar to nic
scp_s.sh nic_arm.tar $slot /data/
scp_s.sh nic_util.tar $slot /data/

# ssh to nic
# ARM ssh session
ssh_s.sh $slot
cd /data
tar xf nic_arm.tar
tar xf nic_util.tar
sync
# quit nic ssh

# Redo nic_test command
./nic_test.py -setup_multi -slot_list $slot -mgmt

# ssh to nic
ssh_s.sh $slot
cd /data/nic_arm/nic/asic_src/ip/cosim/tclsh
./diag.exe
source .tclrc.diag.elb.arm
