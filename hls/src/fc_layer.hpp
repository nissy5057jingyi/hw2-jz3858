/* Copyright 2019 Columbia University, SLD Group */
/* Contact: paolo@cs.columbia.edu                */

#ifndef __FC_LAYER_H__
#define __FC_LAYER_H__

#include "utils.hpp"
#include "fc_layer_datatypes.hpp"

SC_MODULE(fc_layer) {
    public :
    // @TODO: Implement the module fc_layer here.

    // Clock and reset
    sc_in_clk clk;
    sc_in<bool> rst;

    // Configuration interface
    sc_in<fc_layer_conf_t> conf_info_i;
    sc_in<bool> conf_done_i;
    sc_out<bool> conf_done;

    // Data interface
    sc_port<tlm::tlm_fifo_get_if<float>> data_i;
    sc_port<tlm::tlm_fifo_get_if<float>> weights_i;
    sc_port<tlm::tlm_fifo_get_if<float>> bias_i;
    sc_port<tlm::tlm_fifo_put_if<float>> data_o;

    // Control interface
    sc_out<bool> irq_o;
    sc_in<bool> perf_en;            // Performance counter enable signal

    // Internal signals for synchronization
    sc_signal<bool> load_done;
    sc_signal<bool> compute_done;

    // Internal signals for performance monitoring
    sc_signal<bool> load_flag;
    sc_signal<bool> compute_flag;
    sc_signal<bool> store_flag;

    // Configuration parameters
    fc_layer_conf_t cfg;

    // Data storage
    float* data_in;
    float* weights_in;
    float* bias_in;
    float* data_out;


    // -- Constructor
    SC_CTOR(fc_layer)
    {
        // @TODO: fc_layer thread
        SC_THREAD(process_config);
        sensitive << clk.pos();
        reset_signal_is(rst, false);

        SC_THREAD(process_load);
        sensitive << clk.pos();
        reset_signal_is(rst, false);

        SC_THREAD(process_compute);
        sensitive << clk.pos();
        reset_signal_is(rst, false);

        SC_THREAD(process_store);
        sensitive << clk.pos();
        reset_signal_is(rst, false);

        SC_THREAD(process_inc_count);
        sensitive << clk.pos();
        reset_signal_is(rst, false);   
    }

    // @TODO
    void process_config(void);
    void process_load(void);
    void process_compute(void);
    void process_store(void);
    void process_inc_count(void);    // -- SystemC Process to increment performance counters

    // @TODO: Use sc_signal based flags to keep track of performance counters
    // -- Performance Counter Registers
    uint32_t *load_count;
    uint32_t *compute_count;
    uint32_t *store_count;
};

#endif // __FC_LAYER_H__
