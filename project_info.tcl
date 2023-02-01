# This is an automatically generated file used by digilent_vivado_checkout.tcl to set project options
proc set_project_properties_post_create_project {proj_name} {
    set project_obj [get_projects $proj_name]
	global repo_path
	set_propert BOARD_PART_REPO_PATHS [file normalize "$repo_path/board_files/"] $project_obj
	set_property "part" "xczu5ev-sfvc784-1-e" $project_obj
	set_property "board_part" "digilentinc.com:gzu_5ev:part0:1.1" $project_obj
	set_property "default_lib" "xil_defaultlib" $project_obj
	set_property "simulator_language" "Mixed" $project_obj
	set_property "target_language" "Verilog" $project_obj
}

proc set_project_properties_pre_add_repo {proj_name} {
    set project_obj [get_projects $proj_name]
    # default nothing
}

proc set_project_properties_post_create_runs {proj_name} {
    set project_obj [get_projects $proj_name]
    # default nothing
}
