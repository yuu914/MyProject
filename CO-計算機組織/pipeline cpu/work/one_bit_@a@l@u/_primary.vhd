library verilog;
use verilog.vl_types.all;
entity one_bit_ALU is
    generic(
        \AND\           : vl_logic_vector(0 to 2) := (Hi0, Hi0, Hi0);
        \OR\            : vl_logic_vector(0 to 2) := (Hi0, Hi0, Hi1);
        ADD             : vl_logic_vector(0 to 2) := (Hi0, Hi1, Hi0);
        SUB             : vl_logic_vector(0 to 2) := (Hi1, Hi1, Hi0);
        \SLT\           : vl_logic_vector(0 to 2) := (Hi1, Hi1, Hi1);
        \SRL\           : vl_logic_vector(0 to 2) := (Hi0, Hi1, Hi1);
        ADDIU           : vl_logic_vector(0 to 2) := (Hi1, Hi0, Hi0)
    );
    port(
        A               : in     vl_logic;
        B               : in     vl_logic;
        Cin             : in     vl_logic;
        ALU_Cout        : out    vl_logic;
        ALU_MUX_Out     : out    vl_logic;
        \Signal\        : in     vl_logic_vector(2 downto 0);
        slt             : in     vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of \AND\ : constant is 1;
    attribute mti_svvh_generic_type of \OR\ : constant is 1;
    attribute mti_svvh_generic_type of ADD : constant is 1;
    attribute mti_svvh_generic_type of SUB : constant is 1;
    attribute mti_svvh_generic_type of \SLT\ : constant is 1;
    attribute mti_svvh_generic_type of \SRL\ : constant is 1;
    attribute mti_svvh_generic_type of ADDIU : constant is 1;
end one_bit_ALU;
