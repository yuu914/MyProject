library verilog;
use verilog.vl_types.all;
entity Memory is
    port(
        rd              : out    vl_logic_vector(31 downto 0);
        MemRead         : in     vl_logic;
        MemWrite        : in     vl_logic;
        wd              : in     vl_logic_vector(31 downto 0);
        addr            : in     vl_logic_vector(31 downto 0);
        clk             : in     vl_logic
    );
end Memory;
