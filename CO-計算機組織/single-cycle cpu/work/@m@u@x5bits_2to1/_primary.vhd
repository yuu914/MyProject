library verilog;
use verilog.vl_types.all;
entity MUX5bits_2to1 is
    port(
        \out\           : out    vl_logic_vector(4 downto 0);
        sel             : in     vl_logic;
        in0             : in     vl_logic_vector(4 downto 0);
        in1             : in     vl_logic_vector(4 downto 0)
    );
end MUX5bits_2to1;
