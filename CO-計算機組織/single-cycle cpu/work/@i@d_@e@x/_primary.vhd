library verilog;
use verilog.vl_types.all;
entity ID_EX is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        enable          : in     vl_logic;
        WBin            : in     vl_logic_vector(1 downto 0);
        MEMORYin        : in     vl_logic_vector(2 downto 0);
        EXin            : in     vl_logic_vector(3 downto 0);
        IDEXin          : in     vl_logic_vector(31 downto 0);
        RD1in           : in     vl_logic_vector(31 downto 0);
        RD2in           : in     vl_logic_vector(31 downto 0);
        Extndin         : in     vl_logic_vector(31 downto 0);
        shamtin         : in     vl_logic_vector(4 downto 0);
        RTin            : in     vl_logic_vector(4 downto 0);
        RDin            : in     vl_logic_vector(4 downto 0);
        functin         : in     vl_logic_vector(5 downto 0);
        jumpaddrin      : in     vl_logic_vector(31 downto 0);
        jumpin          : in     vl_logic;
        WBout           : out    vl_logic_vector(1 downto 0);
        MEMORYout       : out    vl_logic_vector(2 downto 0);
        EXout           : out    vl_logic_vector(3 downto 0);
        IDEXout         : out    vl_logic_vector(31 downto 0);
        RD1out          : out    vl_logic_vector(31 downto 0);
        RD2out          : out    vl_logic_vector(31 downto 0);
        Extndout        : out    vl_logic_vector(31 downto 0);
        RTout           : out    vl_logic_vector(4 downto 0);
        RDout           : out    vl_logic_vector(4 downto 0);
        shamtout        : out    vl_logic_vector(4 downto 0);
        functout        : out    vl_logic_vector(5 downto 0);
        jumpout         : out    vl_logic;
        jumpaddrout     : out    vl_logic_vector(31 downto 0)
    );
end ID_EX;
