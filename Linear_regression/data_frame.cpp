#include "data_frame.h"
#include "linear_regression.h"
#include "parser.h"

#include <iomanip>
#include <sstream>
#include <fstream>


const int WIDTH = 12; //Width of a scientific.
const matrix::size_type MAX_ROWS = 8;

std::ios_base::fmtflags set_output_settings(std::ostream& os){
    const auto CUR = os.flags();
    os << std::scientific;
    os << std::left;
    return CUR;
}

template<typename T>
void print_vec(std::ostream& os, const matrix::vector<T>& vec) {
    for (const T& x : vec)
        os << std::setw(WIDTH) << x << '\t';
    os << '\n';
}


data_frame::data_frame(std::string file) {
    std::ifstream is {file};
    std::string header;
    std::getline(is, header);
    std::stringstream s {header};
    while (s >> header)
        col_names.push_back(header);

    Parse::Parser::make_valid_names(col_names);

    const matrix::size_type dim = col_names.size();

    matrix::vector<double> temp (dim, 0);
    for (double& d : temp)
        is >> d;
    while(is){
        data.data.push_back(temp);
        for (double& d : temp)
            is >> d;
    }
}

std::ostream& operator<<(std::ostream& os, const data_frame& d) {
    const auto CUR = set_output_settings(os);
    
    print_vec(os, d.col_names);
    if (d.data.row_count() <= MAX_ROWS)
        for(const auto& row : d.data) 
            print_vec(os, row);
    else {
        for (matrix::size_type i = 0; i != MAX_ROWS/2; ++i) 
            print_vec(os, d.data[i]);
        os << ".\n.\n";
        for (matrix::size_type i = d.data.row_count() - MAX_ROWS/2; i != d.data.row_count(); ++i)
            print_vec(os, d.data[i]);
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
    const auto VAR = col_std_dev(d.data, MEANS);
    
    os << std::setw(WIDTH) << "Summary" << '\t';
    print_vec(os, d.col_names);
    os << std::setw(WIDTH) << "Min" << '\t';
    print_vec(os, MIN);
    os << std::setw(WIDTH) << "Max" << '\t';
    print_vec(os, MAX);
    os << std::setw(WIDTH) << "Mean" << '\t';
    print_vec(os, MEANS);
    os << std::setw(WIDTH) << "Std Dev" << '\t';
    print_vec(os, VAR);
    os << std::setw(WIDTH) << "Number of observations: " << d.data.row_count() << '\n';
    if (os)
        os.flags(CUR);
}

matrix::size_type data_frame::index (const std::string& col_name) const {
    return Parse::index(col_names, col_name);
}

matrix::size_type data_frame::get_index (const std::string& col_name) const {
    return Parse::get_index(col_names, col_name);
}


matrix::vector<matrix::size_type> data_frame::get_indices(const matrix::vector<std::string>& col_names) const {
    matrix::vector<matrix::size_type> indices(col_names.size());
    for(matrix::size_type i = 0; i != col_names.size(); ++i)
        indices[i] = get_index(col_names[i]);
    return indices;
}


void data_frame::regress(std::string explained, std::string explanatory, std::ostream& out) const {
    const auto CUR = set_output_settings(out);
    
    std::stringstream str {explanatory};
    matrix::vector<std::string> cols;
    for (std::string s; str >> s; )
        cols.push_back(s);
    
    const auto y = this->data.col_select({get_index(explained)});
    const auto X = this->data.col_select(get_indices(cols));

    Linear_regresser lr;
    lr.fit(y, X);

    out << std::setw(WIDTH) << explained << '\t';
    print_vec(out, cols);
    out << std::setw(WIDTH) << "Coeff" << '\t';
    print_vec(out, lr.get_coeff().t()[0]); //get_coeff() is a k times 1 matrix and print_vec only takes a row.
    out << std::setw(WIDTH) << "Std Dev" << '\t';
    print_vec(out, lr.get_coeff_std_dev());
    out << std::setw(WIDTH) << "t-stat" << '\t';
    print_vec(out, lr.get_coeff_t_stat());
    out << std::setw(WIDTH) << "P > |t|" << '\t';
    print_vec(out, lr.get_t_test());

    const auto intervals = lr.get_95_conf_coeff();
    out << std::setw(WIDTH) << "[95% Conf" << '\t';
    for (const auto& inter : intervals)
        out << std::setw(WIDTH) << inter.left << '\t';
    out << '\n' << std::setw(WIDTH) << "interval]" << '\t';
    for (const auto& inter : intervals)
        out << std::setw(WIDTH) << inter.right << '\t';

    out << "\n\n# Obs: " << std::setw(WIDTH) << lr.get_no_obs() << "\t# Reg: " << lr.get_no_reg() 
            << "\nSSE " << lr.get_SSE() << "\tSSR " << lr.get_SSR()
            << "\nSST " << lr.get_SST() << '\n';

    if (out)
        out.flags(CUR);
}

data_frame& data_frame::add_col(std::string name, std::string expr) {
    if (!Parse::Parser::valid_name(name))
        throw Parse::invalid_col_name {name};
    Parse::Parser p(col_names, expr);

    col_names.push_back(name);

    for (matrix::size_type i = 0; i != data.row_count(); ++i)
        data[i].push_back(p.eval(i, data[i]));
    return *this;
}