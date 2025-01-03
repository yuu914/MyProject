library verilog;
use verilog.vl_types.all;
entity reg_file is
    port(
        RD1             : out    vl_logic_vector(31 downto 0);
        RD2             : out    vl_logic_vector(31 downto 0);
        RegWrite        : in     vl_logic;
        RN1             : in     vl_logic_vector(4 downto 0);
        RN2             : in     vl_logic_vector(4 downto 0);
        WN              : in     vl_logic_vector(4 downto 0);
        WD              : in     vl_logic_vector(31 downto 0);
        clk             : in     vl_logic
    );
end reg_file;
