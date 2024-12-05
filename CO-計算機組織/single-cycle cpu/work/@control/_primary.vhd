library verilog;
use verilog.vl_types.all;
entity Control is
    generic(
        R_type          : vl_logic_vector(0 to 5) := (Hi0, Hi0, Hi0, Hi0, Hi0, Hi0);
        lw              : vl_logic_vector(0 to 5) := (Hi1, Hi0, Hi0, Hi0, Hi1, Hi1);
        sw              : vl_logic_vector(0 to 5) := (Hi1, Hi0, Hi1, Hi0, Hi1, Hi1);
        beq             : vl_logic_vector(0 to 5) := (Hi0, Hi0, Hi0, Hi1, Hi0, Hi0);
        j               : vl_logic_vector(0 to 5) := (Hi0, Hi0, Hi0, Hi0, Hi1, Hi0);
        maddu           : vl_logic_vector(0 to 5) := (Hi0, Hi1, Hi1, Hi1, Hi0, Hi0);
        addiu           : vl_logic_vector(0 to 5) := (Hi0, Hi0, Hi1, Hi0, Hi0, Hi1)
    );
    port(
        b               : out    vl_logic_vector(1 downto 0);
        regdst          : out    vl_logic;
        ALUsrc          : out    vl_logic;
        Branch          : out    vl_logic;
        Memread         : out    vl_logic;
        Memwrite        : out    vl_logic;
        regwrite        : out    vl_logic;
        Memtoreg        : out    vl_logic;
        Jump            : out    vl_logic;
        extendselect    : out    vl_logic;
        ALUop           : out    vl_logic_vector(1 downto 0);
        op              : in     vl_logic_vector(5 downto 0)
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of R_type : constant is 1;
    attribute mti_svvh_generic_type of lw : constant is 1;
    attribute mti_svvh_generic_type of sw : constant is 1;
    attribute mti_svvh_generic_type of beq : constant is 1;
    attribute mti_svvh_generic_type of j : constant is 1;
    attribute mti_svvh_generic_type of maddu : constant is 1;
    attribute mti_svvh_generic_type of addiu : constant is 1;
end Control;
