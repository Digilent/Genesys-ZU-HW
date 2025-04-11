proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "ZmodAwgAxiConfiguration" "NUM_INSTANCES" "DEVICE_ID"  "C_s_axi_control_BASEADDR" "C_s_axi_control_HIGHADDR"
}
