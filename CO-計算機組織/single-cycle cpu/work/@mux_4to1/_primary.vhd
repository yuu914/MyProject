library verilog;
use verilog.vl_types.all;
entity Mux_4to1 is
    port(
        ALU_MUX_Out     : out    vl_logic;
        in4             : in     vl_logic;
        in3             : in     vl_logic;
        in2             : in     vl_logic;
        in1             : in     vl_logic;
        in0             : in     vl_logic;
        \Signal\        : in     vl_logic_vector(2 downto 0)
    );
end Mux_4to1;
