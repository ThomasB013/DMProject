#include "data_frame.h"

#include <iomanip>
#include <algorithm>

const int WIDTH = 12; //Width of a scientific.
const matrix::size_type MAX_ROWS = 8;

std::ios_base::fmtflags set_output_settings(std::ostream& os){
    const auto CUR = os.flags();
    os << std::scientific;
    os << std::left;
    return CUR;
}

void print_col_names(std::ostream& os, const data_frame& d) {
    for (const std::string& n : d.col_names)
        os << std::setw(WIDTH) << n << '\t';
    os << '\n';
}

void print_row(std::ostream& os, const matrix::vector<double>& row) {
    for (double entry : row)
        os << std::setw(WIDTH) << entry << '\t';
    os << '\n';
}

std::ostream& operator<<(std::ostream& os, const data_frame& d) {
    const auto CUR = set_output_settings(os);
    
    print_col_names(os, d);
    if (d.data.row_count() <= MAX_ROWS)
        for(const auto& row : d.data) 
            print_row(os, row);
    else {
        for (matrix::size_type i = 0; i != MAX_ROWS/2; ++i) 
            print_row(os, d.data[i]);
        os << ".\n.\n";
        for (matrix::size_type i = d.data.row_count() - MAX_ROWS/2; i != d.data.row_count(); ++i)
            print_row(os, d.data[i]);
    }

    if (os) 
        os.flags(CUR);
    return os;
}

void print_summary(std::ostream& os, const data_frame& d) {
    const auto CUR = set_output_settings(os);

    const auto MAX = col_max(d.data);
    const auto MIN = col_mins(d.data);
    const auto MEANS = col_means(d.data);
    const auto VAR = col_var(d.data, MEANS);
    
    os << std::setw(WIDTH) << "Summary" << '\t';
    print_col_names(os, d);
    os << std::setw(WIDTH) << "Max" << '\t';
    print_row(os, MAX);
    os << std::setw(WIDTH) << "Min" << '\t';
    print_row(os, MIN);
    os << std::setw(WIDTH) << "Mean" << '\t';
    print_row(os, MEANS);
    os << std::setw(WIDTH) << "Variance" << '\t';
    print_row(os, VAR);
    os << std::setw(WIDTH) << "Number of observations: " << d.data.row_count() << '\n';
    if (os)
        os.flags(CUR);
}

matrix::size_type data_frame::index (const std::string& col_name) const {
    return std::find(col_names.begin(), col_names.end(), col_name) - col_names.begin();
}

matrix::size_type data_frame::get_index (const std::string& col_name) const {
    const auto POS = index(col_name);
    if (POS == col_names.size())
        throw col_name_not_found {"Column name " + col_name + " not found."};
    return POS;
}


matrix::vector<matrix::size_type> data_frame::get_indices(const matrix::vector<std::string>& col_names) const {
    matrix::vector<matrix::size_type> indices(col_names.size());
    for(matrix::size_type i = 0; i != col_names.size(); ++i)
        indices[i] = get_index(col_names[i]);
    return indices;
}

data_frame& data_frame::add_col(std::string name, std::string expr) {

    return *this;
}