library verilog;
use verilog.vl_types.all;
entity EX_MEM is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        enable          : in     vl_logic;
        WBin            : in     vl_logic_vector(1 downto 0);
        MEMORYin        : in     vl_logic_vector(2 downto 0);
        totalALUin      : in     vl_logic_vector(31 downto 0);
        RD2in           : in     vl_logic_vector(31 downto 0);
        addin           : in     vl_logic_vector(31 downto 0);
        WNin            : in     vl_logic_vector(4 downto 0);
        ZEROin          : in     vl_logic;
        jumpin          : in     vl_logic;
        jumpaddrin      : in     vl_logic_vector(31 downto 0);
        WBout           : out    vl_logic_vector(1 downto 0);
        MEMORYout       : out    vl_logic_vector(2 downto 0);
        totalALUout     : out    vl_logic_vector(31 downto 0);
        RD2out          : out    vl_logic_vector(31 downto 0);
        addout          : out    vl_logic_vector(31 downto 0);
        WNout           : out    vl_logic_vector(4 downto 0);
        ZEROout         : out    vl_logic;
        jumpout         : out    vl_logic;
        jumpaddrout     : out    vl_logic_vector(31 downto 0)
    );
end EX_MEM;
