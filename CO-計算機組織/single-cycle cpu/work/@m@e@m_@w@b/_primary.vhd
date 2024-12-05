library verilog;
use verilog.vl_types.all;
entity MEM_WB is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        enable          : in     vl_logic;
        WBin            : in     vl_logic_vector(1 downto 0);
        DMin            : in     vl_logic_vector(31 downto 0);
        ADDRin          : in     vl_logic_vector(31 downto 0);
        WNin            : in     vl_logic_vector(4 downto 0);
        WBout           : out    vl_logic_vector(1 downto 0);
        DMout           : out    vl_logic_vector(31 downto 0);
        ADDRout         : out    vl_logic_vector(31 downto 0);
        WNout           : out    vl_logic_vector(4 downto 0)
    );
end MEM_WB;
