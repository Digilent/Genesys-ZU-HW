
            # Disable timing analysis for clock domain crossing dedicated modules
            set_false_path -through [get_pins -filter {NAME =~ *SyncAsync*/oSyncStages_reg[*]/D} -hier]
            set_false_path -through [get_pins -filter {NAME =~ *SyncAsync*/oSyncStages*/PRE || NAME =~ *SyncAsync*/oSyncStages*/CLR} -hier]

            set_false_path -through [get_pins -filter {NAME =~ *InstHandshake*/*/CLR} -hier]
            set_false_path -from [get_cells -hier -filter {NAME =~ *InstHandshake*/iData_int_reg[*]}] -to [get_cells -hier -filter {NAME=~ *InstHandshake*/oData_reg[*]}]
            
            # are async_regs needed?
        
