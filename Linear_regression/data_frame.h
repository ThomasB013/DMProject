#pragma once

#include <string>
#include <iostream>
#include "helper.h"

/*  This file contains a data_frame which provides user friendly regression analysis.
*/

struct data_frame {
    explicit data_frame() {}
    explicit data_frame(std::string file); //Might change column names. See parser.h

    matrix::size_type index (const std::string& col_name) const; //does not throw exception. Returns col_names.size() on not found.
    matrix::size_type get_index (const std::string& col_name) const; //does throw exception.
    matrix::vector<matrix::size_type> get_indices(const matrix::vector<std::string>& col_names) const;
    void regress(std::string explained, std::string explanatory, std::ostream& out = std::cout ) const;
    data_frame& add_col(std::string name, std::string expr);


    matrix::vector<std::string> col_names;
    matrix data;
};

std::ostream& operator<<(std::ostream& os, const data_frame& d);

void print_summary(std::ostream& os, const data_frame& d);