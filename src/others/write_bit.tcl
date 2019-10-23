# Call this script in the following way:
# vivado -mode batch -notrace -source write_bit.tcl -tclargs <path_to_bitstream>
open_hw

connect_hw_server

open_hw_target

set current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false $current_hw_device

set bit_file_path [lindex $argv 0]
set_property PROGRAM.FILE $bit_file_path $current_hw_device

if { [catch {program_hw_devices $current_hw_device}] } {
	puts "Wrong bitstream!"
	exit 1
}
refresh_hw_device $current_hw_device

set done_status [get_property REGISTER.IR.BIT05_DONE $current_hw_device]

if {$done_status != 1} {
	puts "Bitstream programming FAILED!"
	exit 1
} else {
	puts "Bitstream programming SUCCEEDED!"
	exit 0
}
