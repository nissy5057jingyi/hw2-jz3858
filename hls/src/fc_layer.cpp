/* Copyright 2019 Columbia University, SLD Group */
/* Contact: paolo@cs.columbia.edu                */

// Class header file
#include "fc_layer.hpp"

// -- Functions (kernel)

#include "fc_layer_functions.hpp"

void fc_layer::process_config(void) {
    // @TODO: Implement the reset here.
    if (!rst.read()) {
        conf_done.write(false);
        return;
    }

    // @TODO: Read the configuration parameters:
    //  - conf_info_i: contains the config. parameters.
    //  - conf_done_i: tells you when they are ready.
    while (!conf_done_i.read()) {
        wait();
    }
    cfg = conf_info_i.read();


    // Allocate necessary memory
    // Note that this is not possible in hardware!
    data_in = new float[cfg.in_size];
    weights_in = new float[cfg.num_w_cols * cfg.num_w_rows];
    bias_in = new float[cfg.out_size];
    data_out = new float[cfg.out_size];

    REPORT_INFO("CONFIG DONE");

    // @TODO: Flag that the process_config is done.
    conf_done.write(true);

    // Reset restarts SC_THREAD
    do {
        wait();
    } while (true);
}

void fc_layer::process_load(void) {
    // @TODO: Implement the reset here.
    if (!rst.read()) {
        load_done.write(false);
        return;
    }

    // @TODO: Wait for the process_config to be done.
    while (!conf_done.read()) {
        wait();
    }

    // @TODO: Flag to the increment counter process through internal flag signal
    // that load functionality started and synchronize by waiting
    load_flag.write(true);
    wait();

    // @TODO: Send the data by using the function get()
    //  on the input channel of the fc_layer module.
    for (unsigned i = 0; i < cfg.in_size; i++) {
        data_in[i] = data_i.get();
    }

    for (unsigned i = 0; i < cfg.num_w_cols * cfg.num_w_rows; i++) {
        weights_in[i] = weights_i.get();
    }

    for (unsigned i = 0; i < cfg.out_size; i++) {
        bias_in[i] = bias_i.get();
    }

    wait();

    // @TODO: Flag to the increment counter process through internal flag
    // signal that load functionality is done
    load_flag.write(false);

    REPORT_INFO("LOAD DONE");

    // @TODO: Flag that the process_load is done.
    load_done.write(true);

    // Reset restarts the SC_THREAD.
    do {
        wait();
    } while (true);
}

void fc_layer::process_compute(void) {
    // @TODO: Implement the reset here.
    if (!rst.read()) {
        compute_done.write(false);
        return;
    }

    // @TODO: Wait for the process_config to be done.
    while (!conf_done.read()) {
        wait();
    }

    // @TODO: --> Wait for the process_load to be done.
    while (!load_done.read()) {
        wait();
    }

    // @TODO: Flag to the increment counter process through an 
    // internal flag signal that the compute functionality has started 
    compute_flag.write(true);


    // @TODO: Call the function fc_compute() (fc_layer_functions.hpp).
    //  note: this can be done because we have access to the entire data
    //  set. Start thinking about how you would have to split computation
    //  when only portions of the dataset can be accessed by the compute
    //  process. It will become very useful later in the semester.
    fc_compute(data_in, weights_in, bias_in, data_out,
        cfg.in_size, cfg.out_size,
        cfg.num_w_cols, cfg.num_w_rows);

    wait();

    // @TODO: Flag to the increment counter process through internal flag signal
    // that compute functionality is done
    compute_flag.write(false);

    REPORT_INFO("COMPUTE DONE");

    // @TODO: Flag that the process_compute is done.
    compute_done.write(true);

    // Reset restarts the SC_THREAD.
    do {
        wait();
    } while (true);
}

void fc_layer::process_store(void) {
    // @TODO: Implement the reset here.
    if (!rst.read()) {
        irq_o.write(false);
        return;
    }

    // @TODO: Wait for the process_config to be done.
    while (!conf_done.read()) {
        wait();
    }

    // @TODO: --> Wait for the process_compute to be done.
    while (!compute_done.read()) {
        wait();
    }

    // @TODO: Flag to the increment counter process through internal flag signal
    // that store functionality started and syncronize by waiting
    store_flag.write(true);
    wait();

    // @TODO: Send the data by using the function put()
    //  on the output channel of the fc_layer module. 
    for (unsigned i = 0; i < cfg.out_size; i++) {
        data_o.put(data_out[i]);
    }

    wait();

    // @TODO: Flag to the increment counter process through internal flag signal
    // that store functionality is done
    store_flag.write(false);

    REPORT_INFO("STORE DONE");
 
    // @TODO: If performance monitoring is enabled, then report the performance counter values for all 3 counters
    if (perf_en.read()) {
        REPORT_INFO(STR("Load count: ", *load_count));
        REPORT_INFO(STR("Compute count: ", *compute_count));
        REPORT_INFO(STR("Store count: ", *store_count));
    }
  
    // @TODO: Flag that the module is done (irq_o).
    irq_o.write(true);

    // Free up the memory with "delete[]".
    //  note: this is meaningless in hardware!
    delete[] data_in;
    delete[] weights_in;
    delete[] bias_in;
    delete[] data_out;
    delete[] load_count; 
    delete[] compute_count;
    delete[] store_count; 

    // Reset restarts the SC_THREAD.
    do {
        wait();
    } while (true);
}

void fc_layer:: process_inc_count (void) {
    // Reset
    wait();

    // -- Allocate memory and initialize performance counters to 0
    load_count    = new uint32_t(0);
    compute_count = new uint32_t(0);
    store_count   = new uint32_t(0);

    while(true) {
        if (perf_en.read() && load_flag.read()) {
    	    *load_count+=1; 
        }

       	if (perf_en.read() && compute_flag.read()) {
       	    *compute_count+=1;
       	}
	  
       	if (perf_en.read() && store_flag.read()) {
            *store_count+=1;
       	}

        wait();
    } 
}
