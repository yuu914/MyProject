library verilog;
use verilog.vl_types.all;
entity FA is
    port(
        a               : in     vl_logic;
        b               : in     vl_logic;
        c               : in     vl_logic;
        cout            : out    vl_logic;
        sum             : out    vl_logic
    );
end FA;
