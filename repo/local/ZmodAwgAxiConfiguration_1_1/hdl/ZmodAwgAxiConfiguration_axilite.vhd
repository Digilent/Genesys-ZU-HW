library IEEE;
use ieee.std_logic_1164.all;
use ieee.math_real.log2;
use ieee.math_real.ceil;
use ieee.numeric_std.all;

entity ZmodAwgAxiConfiguration_axilite is
generic (
    ADDR_WIDTH : INTEGER := 6;
    DATA_WIDTH : INTEGER := 32;
    NUM_REGS : INTEGER := 10
    );
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;

    awaddr : IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    awvalid : IN STD_LOGIC;
    awready : OUT STD_LOGIC;

    wdata : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    wstrb : IN STD_LOGIC_VECTOR(DATA_WIDTH/8-1 downto 0);
    wvalid : IN STD_LOGIC;
    wready : OUT STD_LOGIC;

    bresp : OUT STD_LOGIC_VECTOR(1 downto 0);
    bvalid : OUT STD_LOGIC;
    bready : IN STD_LOGIC;
    
    araddr : IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    arvalid : IN STD_LOGIC;
    arready : OUT STD_LOGIC;
    
    rdata : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    rresp : OUT STD_LOGIC_VECTOR(1 downto 0);
    rvalid : OUT STD_LOGIC;
    rready : IN STD_LOGIC;

-- Each register gets an input port if read is allowed and an output port if write is allowed
-- This core is bitfield agnostic
    Reg0_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg1_i : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg2_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg3_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg4_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg5_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg6_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg7_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg8_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    Reg9_o : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    interrupt: OUT STD_LOGIC
    );
end;

architecture Behavioral of ZmodAwgAxiConfiguration_axilite is
component write_fsm is
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;
    awvalid : IN STD_LOGIC;
    awready : OUT STD_LOGIC;
    wvalid : IN STD_LOGIC;
    wready : OUT STD_LOGIC;
    bvalid : OUT STD_LOGIC;
    bready : IN STD_LOGIC;
    awreg_en : OUT STD_LOGIC;
    wreg_en : OUT STD_LOGIC
);
end component;

component read_fsm is
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;
    arvalid : IN STD_LOGIC;
    arready : OUT STD_LOGIC;
    rvalid : OUT STD_LOGIC;
    rready : IN STD_LOGIC;
    arreg_en : OUT STD_LOGIC
);
end component;

component address_decode is
generic (
    ADDR_WIDTH : INTEGER := 2;
    NUM_REGS : INTEGER := 4
);
port (
    address : IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    reg_en : OUT STD_LOGIC_VECTOR(NUM_REGS-1 downto 0)
);
end component;

component skid_buffer is
generic (
    DATA_WIDTH : INTEGER := 32
);
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;
    aready : OUT STD_LOGIC;
    avalid : IN STD_LOGIC;
    adata : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    bready : IN STD_LOGIC;
    bvalid : OUT STD_LOGIC;
    bdata : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0)
);
end component;

component axi4lite_register is
generic (
    DATA_WIDTH : INTEGER := 32;
    RESET_VALUE : STD_LOGIC_VECTOR(31 downto 0) := (others => '0')
);
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;
    enable : IN STD_LOGIC;
    wstrb : IN  STD_LOGIC_VECTOR(DATA_WIDTH/8-1 downto 0);
    data_in : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    data_out : OUT  STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0)
);
end component;

    constant RESP_OKAY : STD_LOGIC_VECTOR(1 downto 0) := "00";
    constant RESP_EXOKAY : STD_LOGIC_VECTOR(1 downto 0) := "01";
    constant RESP_SLVERR : STD_LOGIC_VECTOR(1 downto 0) := "10";
    constant RESP_DECERR : STD_LOGIC_VECTOR(1 downto 0) := "11";

    signal awreg : STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    signal arreg : STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    signal arreg_word : STD_LOGIC_VECTOR(ADDR_WIDTH-3 downto 0);

    signal awreg_en : STD_LOGIC;
    signal wreg_en : STD_LOGIC;
    signal arreg_en : STD_LOGIC;

    signal reg_en : STD_LOGIC_VECTOR(NUM_REGS-1 downto 0);

    signal arready_int : STD_LOGIC;
    signal arvalid_int : STD_LOGIC;
    signal araddr_int : STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    signal rdata_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    
    signal awready_int : STD_LOGIC;
    signal awvalid_int : STD_LOGIC;
    signal awaddr_int : STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    signal Reg0_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg0_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg0_enable : STD_LOGIC;
    signal Reg2_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg2_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg2_enable : STD_LOGIC;
    signal Reg3_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg3_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg3_enable : STD_LOGIC;
    signal Reg4_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg4_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg4_enable : STD_LOGIC;
    signal Reg5_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg5_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg5_enable : STD_LOGIC;
    signal Reg6_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg6_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg6_enable : STD_LOGIC;
    signal Reg7_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg7_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg7_enable : STD_LOGIC;
    signal Reg8_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg8_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg8_enable : STD_LOGIC;
    signal Reg9_i : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal Reg9_int : STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
    signal reg9_enable : STD_LOGIC;

begin
    -- Always respond OKAY.
    -- SLVERRs could be implemented in future for (for ex) attempted writes to read-only registers.

    rresp <= RESP_OKAY;
    bresp <= RESP_OKAY;

    -- Control logic, one state machine each for the read and write channels, which control data flow through this module and handshake signals
    write_fsm_inst: write_fsm
        port map (
            clk      => clk,
            reset    => reset,
            awvalid  => awvalid_int,
            awready  => awready_int,
            wvalid   => wvalid,
            wready   => wready,
            bvalid   => bvalid,
            bready   => bready,
            awreg_en => awreg_en,
            wreg_en  => wreg_en
        );

    read_fsm_inst: read_fsm
        port map (
            clk      => clk,
            reset    => reset,
            arvalid  => arvalid_int,
            arready  => arready_int,
            rvalid   => rvalid,
            rready   => rready,
            arreg_en => arreg_en
        );

    addr_decode_inst: address_decode
        generic map (
            NUM_REGS => NUM_REGS,
            ADDR_WIDTH => ADDR_WIDTH
        )
        port map (
            address => awreg,
            reg_en => reg_en
        );
        
    -- Write address
    awbuffer: skid_buffer
        generic map (
            DATA_WIDTH => ADDR_WIDTH
        )
        port map (
            clk    => clk,
            reset  => reset,
            aready => awready,
            avalid => awvalid,
            adata  => awaddr,
            bready => awready_int,
            bvalid => awvalid_int,
            bdata  => awaddr_int
        );

    process (clk)
    begin
        if rising_edge(clk) then
            if reset = '1' then
                awreg <= (others => '0');
            elsif awreg_en = '1' then
                awreg <= awaddr_int;
            else
                awreg <= awreg;
            end if;
        end if;
    end process;
    
    -- Read address register
    -- skid buffer may not be necessary if master can be guaranteed to never send two read address beats in subsequent cycles,
    -- this is likely the case, and might be built into the AXI spec
    -- without further research, its inclusion guarantees that address will not be dropped
    arbuffer: skid_buffer
        generic map (
            DATA_WIDTH => ADDR_WIDTH
        )
        port map (
            clk    => clk,
            reset  => reset,
            aready => arready,
            avalid => arvalid,
            adata  => araddr,
            bready => arready_int,
            bvalid => arvalid_int,
            bdata  => araddr_int
        );

    process (clk)
    begin
        if rising_edge(clk) then
            if reset = '1' then
                arreg <= (others => '0');
            elsif arreg_en = '1' then
                arreg <= araddr_int;
            else
                arreg <= arreg;
            end if;
        end if;
    end process;
    
    arreg_word <= arreg(arreg'length-1 downto 2);

    -- Read data mux
    -- read data is not skid-buffered because the two-cycle loop time of the control logic guarantees that data will never be updated on two consecutive cycles
    rdata <= rdata_int;
    
    rdata_mux: process(arreg_word, Reg0_i, Reg1_i, Reg2_i, Reg3_i, Reg4_i, Reg5_i, Reg6_i, Reg7_i, Reg8_i, Reg9_i)
    begin
        if arreg_word = std_logic_vector(to_unsigned(0, arreg_word'length)) then
            rdata_int <= Reg0_i;
        elsif arreg_word = std_logic_vector(to_unsigned(1, arreg_word'length)) then
            rdata_int <= Reg1_i;
        elsif arreg_word = std_logic_vector(to_unsigned(2, arreg_word'length)) then
            rdata_int <= Reg2_i;
        elsif arreg_word = std_logic_vector(to_unsigned(3, arreg_word'length)) then
            rdata_int <= Reg3_i;
        elsif arreg_word = std_logic_vector(to_unsigned(4, arreg_word'length)) then
            rdata_int <= Reg4_i;
        elsif arreg_word = std_logic_vector(to_unsigned(5, arreg_word'length)) then
            rdata_int <= Reg5_i;
        elsif arreg_word = std_logic_vector(to_unsigned(6, arreg_word'length)) then
            rdata_int <= Reg6_i;
        elsif arreg_word = std_logic_vector(to_unsigned(7, arreg_word'length)) then
            rdata_int <= Reg7_i;
        elsif arreg_word = std_logic_vector(to_unsigned(8, arreg_word'length)) then
            rdata_int <= Reg8_i;
        elsif arreg_word = std_logic_vector(to_unsigned(9, arreg_word'length)) then
            rdata_int <= Reg9_i;
        else
            rdata_int <= (others => '0');
        end if;
    end process rdata_mux;
    
    -- Individual registers
    -- Register 0 instantiation
    reg0_enable <= reg_en(0) and wreg_en;
    Reg0_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg0_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg0_int
        );
    Reg0_o <= Reg0_int;
    -- No input port, output back to the input
    Reg0_i <= Reg0_int;
    -- Register 2 instantiation
    reg2_enable <= reg_en(2) and wreg_en;
    Reg2_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg2_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg2_int
        );
    Reg2_o <= Reg2_int;
    -- No input port, output back to the input
    Reg2_i <= Reg2_int;
    -- Register 3 instantiation
    reg3_enable <= reg_en(3) and wreg_en;
    Reg3_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg3_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg3_int
        );
    Reg3_o <= Reg3_int;
    -- No input port, output back to the input
    Reg3_i <= Reg3_int;
    -- Register 4 instantiation
    reg4_enable <= reg_en(4) and wreg_en;
    Reg4_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg4_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg4_int
        );
    Reg4_o <= Reg4_int;
    -- No input port, output back to the input
    Reg4_i <= Reg4_int;
    -- Register 5 instantiation
    reg5_enable <= reg_en(5) and wreg_en;
    Reg5_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg5_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg5_int
        );
    Reg5_o <= Reg5_int;
    -- No input port, output back to the input
    Reg5_i <= Reg5_int;
    -- Register 6 instantiation
    reg6_enable <= reg_en(6) and wreg_en;
    Reg6_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg6_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg6_int
        );
    Reg6_o <= Reg6_int;
    -- No input port, output back to the input
    Reg6_i <= Reg6_int;
    -- Register 7 instantiation
    reg7_enable <= reg_en(7) and wreg_en;
    Reg7_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg7_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg7_int
        );
    Reg7_o <= Reg7_int;
    -- No input port, output back to the input
    Reg7_i <= Reg7_int;
    -- Register 8 instantiation
    reg8_enable <= reg_en(8) and wreg_en;
    Reg8_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg8_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg8_int
        );
    Reg8_o <= Reg8_int;
    -- No input port, output back to the input
    Reg8_i <= Reg8_int;
    -- Register 9 instantiation
    reg9_enable <= reg_en(9) and wreg_en;
    Reg9_inst: axi4lite_register
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            RESET_VALUE => std_logic_vector(to_unsigned(0, DATA_WIDTH))
        )
        port map (
            clk      => clk,
            reset    => reset,
            enable   => reg9_enable,
            wstrb    => wstrb,
            data_in  => wdata,
            data_out => Reg9_int
        );
    Reg9_o <= Reg9_int;
    -- No input port, output back to the input
    Reg9_i <= Reg9_int;

-- Fire interrupt on a one cycle delay after write strobes, matching wdata to reg port latency
-- No masking of spurious strobes (like for a write to a RO register) is currently done
process (clk)
begin
    if rising_edge(clk) then
        if reset = '1' then
            interrupt <= '0';
        elsif wreg_en = '1' then
            interrupt <= '1';
        else
            interrupt <= '0';
        end if;
    end if;
end process;

end Behavioral;
