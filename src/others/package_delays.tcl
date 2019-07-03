set partslist { xczu5ev-sfvc784-1-e xczu2cg-sfvc784-1-e }

foreach p $partslist {
  puts $p
  create_project -in_memory -part $p
  set_property design_mode PinPlanning [current_fileset]
  open_io_design -name io_1
  write_csv -force $p.csv
  close_project
}