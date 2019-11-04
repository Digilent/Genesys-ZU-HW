# Call this script in the following way:
# xsct launch_bm_app_zu.tcl <path_to_elf> [<path_to_bitstream> <path_to_psu_init_tcl>]
# If only elf is specified, no psu_init or bitstream download is performed.

switch $argc {
    1 {
        set elf_file_path [lindex $argv 0]
        set bit_file_path {}
        set init_file_path {}
    }
    3 {
        set elf_file_path [lindex $argv 0]
        set bit_file_path [lindex $argv 1]
        set init_file_path [lindex $argv 2]
    }
    default {
        puts "Call this script in the following way:\n\
        xsct launch_bm_app_zu.tcl <path_to_elf> [<path_to_bitstream> <path_to_psu_init_tcl>]"
        return -code error "Wrong number of arguments"
    }
}
if { $elf_file_path != "" && ![file exists $elf_file_path] } {
    return -code error "$elf_file_path does not exist"
}
if { $bit_file_path != "" && ![file exists $bit_file_path] } {
    return -code error "$bit_file_path does not exist"
}
if { $init_file_path != "" && ![file exists $init_file_path] } {
    return -code error "$init_file_path does not exist"
}

set target_APU { name =~ "APU*" && (jtag_cable_serial =~ "210205*" || jtag_cable_serial =~ "210249*") }
set target_A53 { name =~ "*A53 #0" && (jtag_cable_serial =~ "210205*" || jtag_cable_serial =~ "210249*") }

# Connect to local hw_server with fixed address because we expect it to be launched beforehand with HS1/HS2
# cable filters
connect -url TCP:localhost:3121

if { $bit_file_path != "" && $init_file_path != "" } {
    # Select APU, connected to JTAG-HS1 or JTAG-HS2
    # Remove filtering on jtag cables when on-board FTDI gets supported by Xilinx
    targets -set -filter $target_APU

    # Reset while system
    rst -system
    after 3000

    #  Configure the FPGA. When the active target is not a FPGA device, the first FPGA device is configured
    fpga $bit_file_path

    # Source and run init script
    source $init_file_path
    psu_init

    # PS-PL power isolation must be removed and PL reset must be toggled, before the PL address space can be accessed
    # Some delay is needed between these steps
    after 1000
    psu_ps_pl_isolation_removal
    after 1000
    psu_ps_pl_reset_config
}

# Select first A53 core
targets -set -filter $target_A53

# Reset processor
rst -processor

# Download elf file to memory
dow $elf_file_path

# Resume processor core
con