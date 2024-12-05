library verilog;
use verilog.vl_types.all;
entity barrel_shifter is
    port(
        dataA           : in     vl_logic_vector(31 downto 0);
        dataB           : in     vl_logic_vector(4 downto 0);
        SignaltoSHT     : in     vl_logic_vector(2 downto 0);
        dataOut         : out    vl_logic_vector(31 downto 0);
        reset           : in     vl_logic
    );
end barrel_shifter;
