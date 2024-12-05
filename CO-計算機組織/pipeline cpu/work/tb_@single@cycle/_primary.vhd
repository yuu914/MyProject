library verilog;
use verilog.vl_types.all;
entity tb_SingleCycle is
    generic(
        cycle_count     : integer := 100
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of cycle_count : constant is 1;
end tb_SingleCycle;
