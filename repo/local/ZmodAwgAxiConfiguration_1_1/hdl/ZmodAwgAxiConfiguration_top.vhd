library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ZmodAwgAxiConfiguration_top is
generic (
    C_S_AXI_CONTROL_ADDR_WIDTH : INTEGER := 6;
    C_S_AXI_CONTROL_DATA_WIDTH : INTEGER := 32
    );
port (
    s_axi_aclk : IN STD_LOGIC;
    SysClk100 : IN STD_LOGIC;
    DAC_InIO_Clk : IN STD_LOGIC;
    s_axi_areset_n : IN STD_LOGIC;
    sTestMode : OUT STD_LOGIC;
    sDAC_EnIn : OUT STD_LOGIC;
    sExtCh1Scale : OUT STD_LOGIC;
    sExtCh2Scale : OUT STD_LOGIC;
    sInitDoneDAC : IN STD_LOGIC;
    sConfigError : IN STD_LOGIC;
    cExtCh1LgMultCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh1LgAddCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh1HgMultCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh1HgAddCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh2LgMultCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh2LgAddCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh2HgMultCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    cExtCh2HgAddCoeff : OUT STD_LOGIC_VECTOR (17 downto 0);
    s_axi_control_AWVALID : IN STD_LOGIC;
    s_axi_control_AWREADY : OUT STD_LOGIC;
    s_axi_control_AWADDR : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_ADDR_WIDTH-1 downto 0);
    s_axi_control_WVALID : IN STD_LOGIC;
    s_axi_control_WREADY : OUT STD_LOGIC;
    s_axi_control_WDATA : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    s_axi_control_WSTRB : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH/8-1 downto 0);
    s_axi_control_ARVALID : IN STD_LOGIC;
    s_axi_control_ARREADY : OUT STD_LOGIC;
    s_axi_control_ARADDR : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_ADDR_WIDTH-1 downto 0);
    s_axi_control_RVALID : OUT STD_LOGIC;
    s_axi_control_RREADY : IN STD_LOGIC;
    s_axi_control_RDATA : OUT STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    s_axi_control_RRESP : OUT STD_LOGIC_VECTOR (1 downto 0);
    s_axi_control_BVALID : OUT STD_LOGIC;
    s_axi_control_BREADY : IN STD_LOGIC;
    s_axi_control_BRESP : OUT STD_LOGIC_VECTOR (1 downto 0)
    );
end;

architecture Behavioral of ZmodAwgAxiConfiguration_top is
component ZmodAwgAxiConfiguration_axilite is
generic (
    ADDR_WIDTH : INTEGER := C_S_AXI_CONTROL_ADDR_WIDTH;
    DATA_WIDTH : INTEGER := C_S_AXI_CONTROL_DATA_WIDTH
);
port (
    clk : IN STD_LOGIC;
    reset : IN STD_LOGIC;
    awaddr : IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    awvalid : IN STD_LOGIC;
    awready : OUT STD_LOGIC;
    wdata : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    wstrb : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH/8-1 downto 0);
    wvalid : IN STD_LOGIC;
    wready : OUT STD_LOGIC;
    bresp : OUT STD_LOGIC_VECTOR(1 downto 0);
    bvalid : OUT STD_LOGIC;
    bready : IN STD_LOGIC;
    araddr : IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 downto 0);
    arvalid : IN STD_LOGIC;
    arready : OUT STD_LOGIC;
    rdata : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    rresp : OUT STD_LOGIC_VECTOR(1 downto 0);
    rvalid : OUT STD_LOGIC;
    rready : IN STD_LOGIC;
    Reg0_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg0_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg1_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg2_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg2_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg3_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg3_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg4_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg4_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg5_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg5_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg6_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg6_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg7_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg7_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg8_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg8_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg9_i : IN STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    Reg9_o : OUT STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    interrupt: OUT STD_LOGIC
);
end component;

component HandshakeData is
generic (
    kDataWidth : natural := 8
);
port (
    InClk : in STD_LOGIC;
    OutClk : in STD_LOGIC;
    iData : in STD_LOGIC_VECTOR (kDataWidth-1 downto 0);
    oData : out STD_LOGIC_VECTOR (kDataWidth-1 downto 0);
    iPush : in STD_LOGIC;
    iRdy : out STD_LOGIC;
    oAck : in STD_LOGIC := '1';
    oValid : out STD_LOGIC;
    aiReset : in std_logic;
    aoReset : in std_logic
);
end component;

component ChangeDetectHandshake is
generic (
    kDataWidth : natural := 8
);
port (
    InClk : in STD_LOGIC;
    OutClk : in STD_LOGIC;
    iData : in STD_LOGIC_VECTOR (kDataWidth-1 downto 0);
    oData : out STD_LOGIC_VECTOR (kDataWidth-1 downto 0);
    iRdy : out STD_LOGIC;
    oValid : out STD_LOGIC;
    aiReset : in std_logic;
    aoReset : in std_logic
);
end component;

component ResetBridge is
generic (
    kPolarity : std_logic := '1'
);
port (
    aRst : in STD_LOGIC; -- asynchronous reset; active-high, if kPolarity=1
    OutClk : in STD_LOGIC;
    oRst : out STD_LOGIC
);
end component;

-- HLS interrupt flag
signal lInterrupt : STD_LOGIC;

-- Reset signals for each clock domain
signal lRst_n : STD_LOGIC;
signal lRst : STD_LOGIC;
signal sRst_n : STD_LOGIC;
signal sRst : STD_LOGIC;
signal cRst_n : STD_LOGIC;
signal cRst : STD_LOGIC;

-- Internal signals for ports
signal sReg0 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg0 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal sReg1 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg1 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg2 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg2 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg3 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg3 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg4 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg4 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg5 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg5 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg6 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg6 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg7 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg7 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg8 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg8 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal cReg9 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
signal lReg9 : STD_LOGIC_VECTOR(C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);

begin

--- Instantiate register file core
ZmodAwgAxiConfiguration_axilite_inst: ZmodAwgAxiConfiguration_axilite port map(
    clk => s_axi_aclk,
    reset => lRst,
    Reg0_i => lReg0,
    Reg0_o => lReg0,
    Reg1_i => lReg1,
    Reg2_i => lReg2,
    Reg2_o => lReg2,
    Reg3_i => lReg3,
    Reg3_o => lReg3,
    Reg4_i => lReg4,
    Reg4_o => lReg4,
    Reg5_i => lReg5,
    Reg5_o => lReg5,
    Reg6_i => lReg6,
    Reg6_o => lReg6,
    Reg7_i => lReg7,
    Reg7_o => lReg7,
    Reg8_i => lReg8,
    Reg8_o => lReg8,
    Reg9_i => lReg9,
    Reg9_o => lReg9,

    awvalid   => s_axi_control_AWVALID,
    awready   => s_axi_control_AWREADY,
    awaddr    => s_axi_control_AWADDR,
    wvalid    => s_axi_control_WVALID,
    wready    => s_axi_control_WREADY,
    wdata     => s_axi_control_WDATA,
    wstrb     => s_axi_control_WSTRB,
    arvalid   => s_axi_control_ARVALID,
    arready   => s_axi_control_ARREADY,
    araddr    => s_axi_control_ARADDR,
    rvalid    => s_axi_control_RVALID,
    rready    => s_axi_control_RREADY,
    rdata     => s_axi_control_RDATA,
    rresp     => s_axi_control_RRESP,
    bvalid    => s_axi_control_BVALID,
    bready    => s_axi_control_BREADY,
    bresp     => s_axi_control_BRESP,
    interrupt => lInterrupt
);

--- Create synchronous resets for each clock
lRst_n <= s_axi_areset_n;
lRst <= not s_axi_areset_n;

s_axi_aclk_to_SysClk100_rst: ResetBridge generic map(
    kPolarity => '0'
)
port map (
    aRst => s_axi_areset_n,
    outClk => SysClk100,
    oRst => sRst_n
);
sRst <= not sRst_n;

s_axi_aclk_to_DAC_InIO_Clk_rst: ResetBridge generic map(
    kPolarity => '0'
)
port map (
    aRst => s_axi_areset_n,
    outClk => DAC_InIO_Clk,
    oRst => cRst_n
);
cRst <= not cRst_n;

--- Instantiate handshake clock domain crossing modules

---- Register 0 input path
---- trigger handshake push on any difference in the input bus
reg0_from_SysClk100_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => SysClk100,
    OutClk => s_axi_aclk,
    iData => sReg0,
    oData => lReg0,
    iRdy => open,
    oValid => open,
    aiReset => sRst,
    aoReset => lRst
);
sReg0(0) <= sTestMode;
sReg0(1) <= sDAC_EnIn;
sReg0(2) <= sExtCh1Scale;
sReg0(3) <= sExtCh2Scale;
sReg0(4) <= '0';
sReg0(5) <= '0';
sReg0(6) <= '0';
sReg0(7) <= '0';
sReg0(8) <= '0';
sReg0(9) <= '0';
sReg0(10) <= '0';
sReg0(11) <= '0';
sReg0(12) <= '0';
sReg0(13) <= '0';
sReg0(14) <= '0';
sReg0(15) <= '0';
sReg0(16) <= '0';
sReg0(17) <= '0';
sReg0(18) <= '0';
sReg0(19) <= '0';
sReg0(20) <= '0';
sReg0(21) <= '0';
sReg0(22) <= '0';
sReg0(23) <= '0';
sReg0(24) <= '0';
sReg0(25) <= '0';
sReg0(26) <= '0';
sReg0(27) <= '0';
sReg0(28) <= '0';
sReg0(29) <= '0';
sReg0(30) <= '0';
sReg0(31) <= '0';

---- Register 0 output path
---- trigger handshake on interrupt
reg0_from_s_axi_aclk_to_SysClk100_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => SysClk100,
    iData => lReg0,
    oData => sReg0,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => sRst
);
sTestMode <= sReg0(0);
sDAC_EnIn <= sReg0(1);
sExtCh1Scale <= sReg0(2);
sExtCh2Scale <= sReg0(3);

---- Register 1 input path
---- trigger handshake push on any difference in the input bus
reg1_from_SysClk100_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => SysClk100,
    OutClk => s_axi_aclk,
    iData => sReg1,
    oData => lReg1,
    iRdy => open,
    oValid => open,
    aiReset => sRst,
    aoReset => lRst
);
sReg1(0) <= sInitDoneDAC;
sReg1(1) <= sConfigError;
sReg1(2) <= '0';
sReg1(3) <= '0';
sReg1(4) <= '0';
sReg1(5) <= '0';
sReg1(6) <= '0';
sReg1(7) <= '0';
sReg1(8) <= '0';
sReg1(9) <= '0';
sReg1(10) <= '0';
sReg1(11) <= '0';
sReg1(12) <= '0';
sReg1(13) <= '0';
sReg1(14) <= '0';
sReg1(15) <= '0';
sReg1(16) <= '0';
sReg1(17) <= '0';
sReg1(18) <= '0';
sReg1(19) <= '0';
sReg1(20) <= '0';
sReg1(21) <= '0';
sReg1(22) <= '0';
sReg1(23) <= '0';
sReg1(24) <= '0';
sReg1(25) <= '0';
sReg1(26) <= '0';
sReg1(27) <= '0';
sReg1(28) <= '0';
sReg1(29) <= '0';
sReg1(30) <= '0';
sReg1(31) <= '0';

---- Register 2 input path
---- trigger handshake push on any difference in the input bus
reg2_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg2,
    oData => lReg2,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg2(17 downto 0) <= cExtCh1LgMultCoeff;
cReg2(18) <= '0';
cReg2(19) <= '0';
cReg2(20) <= '0';
cReg2(21) <= '0';
cReg2(22) <= '0';
cReg2(23) <= '0';
cReg2(24) <= '0';
cReg2(25) <= '0';
cReg2(26) <= '0';
cReg2(27) <= '0';
cReg2(28) <= '0';
cReg2(29) <= '0';
cReg2(30) <= '0';
cReg2(31) <= '0';

---- Register 2 output path
---- trigger handshake on interrupt
reg2_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg2,
    oData => cReg2,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh1LgMultCoeff <= cReg2(17 downto 0);

---- Register 3 input path
---- trigger handshake push on any difference in the input bus
reg3_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg3,
    oData => lReg3,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg3(17 downto 0) <= cExtCh1LgAddCoeff;
cReg3(18) <= '0';
cReg3(19) <= '0';
cReg3(20) <= '0';
cReg3(21) <= '0';
cReg3(22) <= '0';
cReg3(23) <= '0';
cReg3(24) <= '0';
cReg3(25) <= '0';
cReg3(26) <= '0';
cReg3(27) <= '0';
cReg3(28) <= '0';
cReg3(29) <= '0';
cReg3(30) <= '0';
cReg3(31) <= '0';

---- Register 3 output path
---- trigger handshake on interrupt
reg3_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg3,
    oData => cReg3,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh1LgAddCoeff <= cReg3(17 downto 0);

---- Register 4 input path
---- trigger handshake push on any difference in the input bus
reg4_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg4,
    oData => lReg4,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg4(17 downto 0) <= cExtCh1HgMultCoeff;
cReg4(18) <= '0';
cReg4(19) <= '0';
cReg4(20) <= '0';
cReg4(21) <= '0';
cReg4(22) <= '0';
cReg4(23) <= '0';
cReg4(24) <= '0';
cReg4(25) <= '0';
cReg4(26) <= '0';
cReg4(27) <= '0';
cReg4(28) <= '0';
cReg4(29) <= '0';
cReg4(30) <= '0';
cReg4(31) <= '0';

---- Register 4 output path
---- trigger handshake on interrupt
reg4_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg4,
    oData => cReg4,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh1HgMultCoeff <= cReg4(17 downto 0);

---- Register 5 input path
---- trigger handshake push on any difference in the input bus
reg5_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg5,
    oData => lReg5,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg5(17 downto 0) <= cExtCh1HgAddCoeff;
cReg5(18) <= '0';
cReg5(19) <= '0';
cReg5(20) <= '0';
cReg5(21) <= '0';
cReg5(22) <= '0';
cReg5(23) <= '0';
cReg5(24) <= '0';
cReg5(25) <= '0';
cReg5(26) <= '0';
cReg5(27) <= '0';
cReg5(28) <= '0';
cReg5(29) <= '0';
cReg5(30) <= '0';
cReg5(31) <= '0';

---- Register 5 output path
---- trigger handshake on interrupt
reg5_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg5,
    oData => cReg5,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh1HgAddCoeff <= cReg5(17 downto 0);

---- Register 6 input path
---- trigger handshake push on any difference in the input bus
reg6_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg6,
    oData => lReg6,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg6(17 downto 0) <= cExtCh2LgMultCoeff;
cReg6(18) <= '0';
cReg6(19) <= '0';
cReg6(20) <= '0';
cReg6(21) <= '0';
cReg6(22) <= '0';
cReg6(23) <= '0';
cReg6(24) <= '0';
cReg6(25) <= '0';
cReg6(26) <= '0';
cReg6(27) <= '0';
cReg6(28) <= '0';
cReg6(29) <= '0';
cReg6(30) <= '0';
cReg6(31) <= '0';

---- Register 6 output path
---- trigger handshake on interrupt
reg6_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg6,
    oData => cReg6,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh2LgMultCoeff <= cReg6(17 downto 0);

---- Register 7 input path
---- trigger handshake push on any difference in the input bus
reg7_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg7,
    oData => lReg7,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg7(17 downto 0) <= cExtCh2LgAddCoeff;
cReg7(18) <= '0';
cReg7(19) <= '0';
cReg7(20) <= '0';
cReg7(21) <= '0';
cReg7(22) <= '0';
cReg7(23) <= '0';
cReg7(24) <= '0';
cReg7(25) <= '0';
cReg7(26) <= '0';
cReg7(27) <= '0';
cReg7(28) <= '0';
cReg7(29) <= '0';
cReg7(30) <= '0';
cReg7(31) <= '0';

---- Register 7 output path
---- trigger handshake on interrupt
reg7_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg7,
    oData => cReg7,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh2LgAddCoeff <= cReg7(17 downto 0);

---- Register 8 input path
---- trigger handshake push on any difference in the input bus
reg8_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg8,
    oData => lReg8,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg8(17 downto 0) <= cExtCh2HgMultCoeff;
cReg8(18) <= '0';
cReg8(19) <= '0';
cReg8(20) <= '0';
cReg8(21) <= '0';
cReg8(22) <= '0';
cReg8(23) <= '0';
cReg8(24) <= '0';
cReg8(25) <= '0';
cReg8(26) <= '0';
cReg8(27) <= '0';
cReg8(28) <= '0';
cReg8(29) <= '0';
cReg8(30) <= '0';
cReg8(31) <= '0';

---- Register 8 output path
---- trigger handshake on interrupt
reg8_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg8,
    oData => cReg8,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh2HgMultCoeff <= cReg8(17 downto 0);

---- Register 9 input path
---- trigger handshake push on any difference in the input bus
reg9_from_DAC_InIO_Clk_to_s_axi_aclk_InstHandshake: ChangeDetectHandshake 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => DAC_InIO_Clk,
    OutClk => s_axi_aclk,
    iData => cReg9,
    oData => lReg9,
    iRdy => open,
    oValid => open,
    aiReset => cRst,
    aoReset => lRst
);
cReg9(17 downto 0) <= cExtCh2HgAddCoeff;
cReg9(18) <= '0';
cReg9(19) <= '0';
cReg9(20) <= '0';
cReg9(21) <= '0';
cReg9(22) <= '0';
cReg9(23) <= '0';
cReg9(24) <= '0';
cReg9(25) <= '0';
cReg9(26) <= '0';
cReg9(27) <= '0';
cReg9(28) <= '0';
cReg9(29) <= '0';
cReg9(30) <= '0';
cReg9(31) <= '0';

---- Register 9 output path
---- trigger handshake on interrupt
reg9_from_s_axi_aclk_to_DAC_InIO_Clk_InstHandshake: HandshakeData 
generic map (
    kDataWidth => C_S_AXI_CONTROL_DATA_WIDTH
)
port map(
    InClk => s_axi_aclk,
    OutClk => DAC_InIO_Clk,
    iData => lReg9,
    oData => cReg9,
    iPush => lInterrupt,
    iRdy => open,
    oAck => '1',
    oValid => open,
    aiReset => lRst,
    aoReset => cRst
);
cExtCh2HgAddCoeff <= cReg9(17 downto 0);

end Behavioral;
