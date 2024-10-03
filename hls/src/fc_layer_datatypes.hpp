/* Copyright 2019 Columbia University, SLD Group */

//
// fc_layer_datatypes - Paolo Mantovani
//

#include "systemc.h"

#ifndef __FC_LAYER_DATATYPES_H__
#define __FC_LAYER_DATATYPES_H__

struct fc_layer_conf_t {
    // @TODO: Implement the custom data type here.
    uint32_t in_size;       // Size of the input data
    uint32_t out_size;      // Size of the output data
    uint32_t num_w_cols;    // Number of columns in the weight matrix
    uint32_t num_w_rows;    // Number of rows in the weight matrix

    // Default constructor
    fc_layer_conf_t() : in_size(0), out_size(0), num_w_cols(0), num_w_rows(0) {}

    // Constructor with parameters
    fc_layer_conf_t(uint32_t in, uint32_t out, uint32_t cols, uint32_t rows)
        : in_size(in), out_size(out), num_w_cols(cols), num_w_rows(rows) {}
    
    // Overload the == operator for comparison
    bool operator==(const fc_layer_conf_t& other) const {
        return (in_size == other.in_size &&
            out_size == other.out_size &&
            num_w_cols == other.num_w_cols &&
            num_w_rows == other.num_w_rows);
};
    // Overload the != operator for comparison
    bool operator!=(const fc_layer_conf_t& other) const {
        return !(*this == other);
    }

    // Implement the sc_trace function for waveform tracing
    friend void sc_trace(sc_trace_file* tf, const fc_layer_conf_t& conf, const std::string& name) {
        sc_trace(tf, conf.in_size, name + ".in_size");
        sc_trace(tf, conf.out_size, name + ".out_size");
        sc_trace(tf, conf.num_w_cols, name + ".num_w_cols");
        sc_trace(tf, conf.num_w_rows, name + ".num_w_rows");
    }
};

#endif // __FC_LAYER_DATATYPES_H__
