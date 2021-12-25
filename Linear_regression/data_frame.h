#pragma once

#include <string>
#include <iostream>
#include "helper.h"

struct col_name_not_found : std::runtime_error { col_name_not_found(const std::string& msg) : std::runtime_error{msg} {}};

struct data_frame {
    
    matrix::vector<std::string> col_names;
    matrix data;
    matrix::size_type index (const std::string& col_name) const;
    matrix::size_type get_index (const std::string& col_name) const;
    matrix::vector<matrix::size_type> get_indices(const matrix::vector<std::string>& col_names) const;
    data_frame& add_col(std::string name, std::string expr);
};

std::ostream& operator<<(std::ostream& os, const data_frame& d);

void print_summary(std::ostream& os, const data_frame& d);