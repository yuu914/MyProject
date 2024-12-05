library verilog;
use verilog.vl_types.all;
entity Multiplier is
    port(
        clk             : in     vl_logic;
        dataA           : in     vl_logic_vector(31 downto 0);
        dataB           : in     vl_logic_vector(31 downto 0);
        SignaltoMUL     : in     vl_logic;
        MULAns          : out    vl_logic_vector(63 downto 0);
        reset           : in     vl_logic
    );
end Multiplier;
