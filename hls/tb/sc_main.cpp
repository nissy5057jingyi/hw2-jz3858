/* Copyright 2019 Columbia University, SLD Group */
/* Contact: paolo@cs.columbia.edu                */

#include "system.hpp"

#define CLOCK_PERIOD 10
#define RESET_PERIOD 51

system_t *sys = NULL;
std::string model_path = "../../models/dwarf7.mojo"; // path of the model
std::string image_name = "airplane";                 // input image (preprocessed)

int sc_main(int argc, char *argv[]) {
    // Kills a Warning when using SC_THREADS
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    if (argc > 1) {
        image_name = std::string(argv[1]);
    }

    // @TODO Define the clock signal
    // and the reset signal here. Use
    // the define above to define the
    // clock period of the clock signal.
    sc_clock clk("clk", sc_time(CLOCK_PERIOD, SC_NS));
    sc_signal<bool> rst;

    // @TODO: Define a signal perf_en to 
    // enable/disable performance monitors in the DUT
    sc_signal<bool> perf_en;

    // @TODO Instantiate the module system_t:
    //   -- specify as parameter model_path
    //   -- specify as parameter image_name
    sys = new system_t("system", model_path, image_name);

    SC_REPORT_INFO("sc_main()", STR("Running DWARF7 for input image ", image_name));
    SC_REPORT_INFO("sc_main()", STR("Clock period is ", CLOCK_PERIOD, " ns\n"));

    // @TODO Connect the clock and reset
    // signal to the module system_t.

    // @TODO Connect the performance enable
    // signal to the module system_t.
    sys->clk(clk);
    sys->rst(rst);

    // @TODO Reset the simulation here.
    //  -- hint: use the rst signal and
    //   the function sc_start
    rst.write(false);
    sc_start(sc_time(RESET_PERIOD, SC_NS));
    rst.write(true);

    // TODO: Enable perofrmance by writing true to the signal
    perf_en.write(true);

    // -- Starting the simulation here

    sc_start();

    return 0;
}
