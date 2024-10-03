/* Copyright 2019 Columbia University, SLD Group */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>

#include "mojo.h"
#include "dwarf.h"
#include "fc_layer_tb.hpp"

// Data structure
mojo::network *cnn;

#define TARGET_LAYER 5

void fc_layer_tb::set_configuration_param() {
    config.num_w_cols = cnn->W[TARGET_LAYER - 1]->cols;

    // @TODO Configure the remaining parameters for the invocation
    //  of the fc_layer, i.e., the parameters that have
    //  been specified in the struct fc_layer_conf_t.

    //  hint: you can find the values of these params
    //   by using the data structure cnn. Remember that
    //   you want to use the data of the previous layer
    //   (i.e., the data for TARGET_LAYER - 1).
    config.in_size = cnn->layer_sets[TARGET_LAYER - 1]->node.size();
    config.out_size = cnn->layer_sets[TARGET_LAYER]->node.size();
    config.num_w_rows = cnn->W[TARGET_LAYER - 1]->rows;
}

void fc_layer_tb::run_pv(int layer, bool fully_connected = false) {
    if (fully_connected)
        fc_compute(cnn->layer_sets[layer]->node.x, cnn->layer_sets[layer - 1]->node.x, cnn->W[layer - 1]->x,
                   cnn->layer_sets[layer]->bias.x, cnn->W[layer - 1]->cols, cnn->W[layer - 1]->rows,
                   cnn->layer_sets[layer]->relu);
    else
        convolution_compute(cnn->layer_sets[layer]->node.x, cnn->layer_sets[layer]->bias.x,
                            cnn->layer_sets[layer - 1]->node.x, cnn->W[layer - 1]->x, cnn->layer_sets[layer]->node.cols,
                            cnn->layer_sets[layer]->node.rows, cnn->layer_sets[layer - 1]->node.chans,
                            cnn->layer_sets[layer]->node.chans,
                            get_pool_size(cnn->layer_sets[layer]->node.cols, cnn->layer_sets[layer]->node.rows,
                                          cnn->layer_sets[layer]->do_pool, cnn->layer_sets[layer]->do_pad),
                            get_pool_stride(cnn->layer_sets[layer]->node.cols, cnn->layer_sets[layer]->node.rows,
                                            cnn->layer_sets[layer]->do_pool, cnn->layer_sets[layer]->do_pad),
                            cnn->layer_sets[layer]->do_pool, cnn->layer_sets[layer]->do_pad);
}

void fc_layer_tb::load_memory() {

    // Read image
    const uint32_t img_size = 3 * 224 * 224;
    float *img = new float[img_size];
    std::string image_path = image_name + ".bin";
    std::ifstream infile(image_path.c_str(), std::ifstream::binary);
    infile.read((char *)img, img_size * sizeof(float));
    infile.close();

    cnn->forward_setup(img);
    delete[] img;
    REPORT_INFO("Classifying image...");

    //
    // Run inference
    //

    // Input layer -> nothing to be done
    // input 34x34 (including pad of 2 pixels), 3 channels

    //
    // Convolution type 1
    //
    // convolution 34x34 (including pad of 2 pixels), kernel
    //  size 3x3, 32 output channels, relu
    run_pv(1);

    //
    // Convolution type 2
    //
    // convolution 34x34 (including pad of 2 pixels), kernel
    // size 3x3, 32 output channels, relu, max pooling 2x2 (pad on output -> 18x18)
    run_pv(2);

    //
    // Convolution type 3
    //
    // convolution 18x18 (including pad of 2 pixels), kernel
    //  size 3x3, 64 output channels, relu, max pooling 2x2 (pad on output -> 10x10)
    run_pv(3);

    //
    // Convolution type 4
    //
    // convolution 10x10 (including pad of 2 pixels), kernel
    //  size 3x3, 128 output channels, relu, max pooling 2x2 (no pad on output -> 4x4)
    run_pv(4);

    //
    // Fully Connected
    //

    // fully_connected 1x1, 2048 input channels, 64 output channels, relu (TARGET_LAYER)
    // @NOTE: This is executed only for validation purposes.
    //  Your SystemC module will implement exactly the same.
    run_pv(5, true);

    // @TODO Read the data computed from the previous layer:
    //   -- hint: cnn->layer_sets[..]->node.x is "data"
    //   -- hint: cnn->layer_sets[..]->bias.x is "bias"
    //   -- hint: cnn->W[..]->x is "weights"
    //   -- Substitute ".." with the proper layer index.
    float* data = cnn->layer_sets[TARGET_LAYER - 1]->node.x;
    float* weights = cnn->W[TARGET_LAYER - 1]->x;
    float* bias = cnn->layer_sets[TARGET_LAYER]->bias.x;

        // @TODO Send the data to the SystemC module fc_layer
        //  -- Send them in this order: data, weights, bias.
        for (uint32_t i = 0; i < config.in_size; i++) {
            data_i->put(data[i]);
        }
        for (uint32_t i = 0; i < config.num_w_cols * config.num_w_rows; i++) {
            weights_i->put(weights[i]);
        }
        for (uint32_t i = 0; i < config.out_size; i++) {
            bias_i->put(bias[i]);
        }

        REPORT_INFO("Load memory completed.");
}

void fc_layer_tb::dump_memory() {
    // @TODO Read the data from the computed layer and
    //   store them in the data structure "cnn". To do
    //   so define a variable "output" and save the
    //   result of fc_layer in the variable "val".
    float *output = cnn->layer_sets[TARGET_LAYER]->node.x;
    for (uint32_t i = 0; i < config.out_size; i++) {
        float val = data_o->get();

            // @NOTE Do not modify this!
            if (output[i] != val)
                ++tot_errors;

        // @NOTE Do not modify this!
        output[i] = val;
    }

    REPORT_INFO("Dump memory completed.");

    // fully_connected 1x1, 10 channels, brokemax
    run_pv(6, true);
}

void fc_layer_tb::validate() {

    float *out = cnn->layer_sets[6]->node.x;

    int first = mojo::arg_max(out, cnn->out_size());
    std::stringstream stm;
    stm << "Results:" << std::endl;
    stm << "  1: label|score: \"" << labels[first] << "\" | " << out[first] << std::endl;
    // find next best
    out[first] = -1;
    int second = mojo::arg_max(out, cnn->out_size());
    stm << "  2: label|score: \"" << labels[second] << "\" | " << out[second];

    REPORT_INFO(STR(stm.str()));

    if (tot_errors == 0) {
        REPORT_INFO("validation succeeded!");
    } else {
        REPORT_INFO("validation failed!");
    }
}

void fc_layer_tb::end_simulation() {
    // Free-up memory
    delete cnn;

    // End simulation
    sc_stop();
}

void fc_layer_tb::process_tb() {
    // Reset
    conf_done_o.write(false);
    wait();
    REPORT_INFO("Reset done.");

    // load DWARF7 model
    cnn = new mojo::network();
    REPORT_INFO("Loading DWARF7 model...");

    if (!cnn->read(model_path)) {
        REPORT_ERROR(STR("Could not read model: ", model_path, "."));
        sc_stop();
    }

    set_configuration_param();

    sc_time begin_time = sc_time_stamp();
    REPORT_INFO("Configuration sent.");
    REPORT_INFO(STR("@", begin_time.value(), " ps BEGIN - DUT"));

    // @TODO Write the configuration here.
    //  hint: use the conf_info_o signal.
    //  hint: use the conf_done_o signal.
    conf_info_o.write(config);
    conf_done_o.write(true);

    // Load data into DUT's memory
    load_memory();

    // Dump data fom DUT's memory
    dump_memory();

    sc_time end_time = sc_time_stamp();
    REPORT_INFO(STR("@", end_time.value(), " ps END - DUT"));

    // Validate
    validate();

    wait();

    // End simulation
    end_simulation();
}
