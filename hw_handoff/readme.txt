----------------------------
-- Hardware
----------------------------
To re-create a Vivado project:
0. Make sure the directory does not already contain a project with the same name. 
   You may run cleanup.cmd to delete everything except the utility files.
1. Open either the Vivado Tcl shell or the Tcl Window in Vivado GUI 
2. cd to the directory where you want the project created. 
   For example: <repo>/proj
3. run: source ./create_project.tcl

To make sure changes to the project are checked into git:
	Export hardware with bitstream to ./hw_handoff/.
	Export block design to ./src/bd/, if there is one.
	If there are changes to the Vivado project settings, go to File -> 
    Write Project TCL. Copy relevant TCL commands to proj/create_project.tcl. 
    This is the only project-relevant file checked into Git.
	Store all the project sources in src/. Design files go into src/hdl/, 
    constraints into src/constraints.
	Any IPs instantiated OUTSIDE BLOCK DESIGNS need to be created in /src/ip. 
    Use the IP Location button in the IP customization wizard to specify a 
    target directory. Only *.xci and *.prj files are checked in from here.
	If using MIG outside block designs, manually move the MIG IP to the 
    src/ip folder.

----------------------------
-- Software
----------------------------
Vitis workspace is versioned in separate repo.

 