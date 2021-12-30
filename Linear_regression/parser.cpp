#include "parser.h"

#include <functional>
#include <algorithm>
#include <math.h>
#include <sstream>


namespace { //Unnamed namespace

struct BinOp : Parse::Expression { //Binary operations
    BinOp(Expression* left, Expression* right, std::function<double(double, double)> f) : l{left}, r{right}, op{f} {}
    ~BinOp() { delete l; delete r; }
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return op(l->eval(i, row), r->eval(i, row)); }
    Expression* l, * r;
    std::function<double(double, double)> op;
};

struct UnOp : Parse::Expression { //Unary operations
    UnOp(Expression* exp, std::function<double(double)> f) :e {exp}, op {f} {}
    ~UnOp() { delete e; }
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return op(e->eval(i, row)); }
    Expression* e;
    std::function<double(double)> op;
};

struct LookUp : Parse::Expression { //Look up the value in the row
    LookUp(matrix::size_type i) : index {i} {}
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return row[index]; }
    matrix::size_type index;
};

struct Index : Parse::Expression { //Evaluates to the index passed in eval. $i in the grammar.
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return static_cast<double>(i); }
};

struct Val : Parse::Expression { //Holds a single value.
    Val(double v) :val(v) {}
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return val; }
    double val;
};

Parse::Expression* expr(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume);

/*term = ( + expr + ) | e | pi | $i | 
    lg term | sqrt term | -term
    [a-Z|_][a-Z|0-9|_]* | [0-9]+(.[0-9]+)? */

//Remember that we don't allow column names to start with the reserved names.
Parse::Expression* term(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume) { 
    i += consume;
    
    if (i >= s.size())
        throw Parse::exp_parse_error("term", s, i);
    //We now now that i is in range. This allows us to use substr(pos, len).
    if (s[i] == '('){ //( + expr + )
        Parse::Expression* e = expr(col_names, s, i, 1);
        if (i < s.size() && s[i] == ')') {
            ++i; //consume ).
            return e;
        }
        else
            throw Parse::exp_parse_error("')'", s, i);
    }
    if (s[i] == 'e') { 
        ++i;
        return new Val { Helper::MATH_E };
    }
    if (s.substr(i, 2) == "pi") {
        i += 2;
        return new Val { Helper::MATH_PI };
    }
    if (s.substr(i, 2) == "$i"){
        i += 2;
        return new Index {};
    }
    if (s[i] == '$')
        throw Parse::exp_parse_error("i after $", s, i);
    if (s.substr(i, 2) == "lg") 
        return new UnOp {term(col_names, s, i, 2), [](double x) -> double { return std::log(x); } };
    if (s.substr(i, 4) == "sqrt")
        return new UnOp {term(col_names, s, i, 4), [](double x) -> double { return std::sqrt(x); } };
    if (s[i] == '-')
        return new UnOp {term(col_names, s, i, 1), std::negate<double>() };
    if (std::isdigit(s[i])) { //read a numerical value.
        std::stringstream valBuf;
        for (; i != s.size() && (std::isdigit(s[i]) || s[i] == '.'); ++i)
            valBuf << s[i];
        double val;
        valBuf >> val;
        return new Val { val };
    }//read a column name. And convert to index.
    std::string name;
    for (; i != s.size() && (std::isalnum(s[i]) || s[i] == '_'); ++i)
        name += s[i];

    size_t ind = Parse::get_index(col_names, name);
    return new LookUp{ ind };
}

//exp = term ^ exp | term
Parse::Expression* exp(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume) {
    i += consume;
    Parse::Expression* t = term(col_names, s, i, 0);
    if (i < s.size() && s[i] == '^')
        return new BinOp {t, exp(col_names, s, i, 1), [](double x, double n) -> double { return std::pow(x, n); } };
    return t;
}

//mult = exp * mult | exp / mult | exp
Parse::Expression* mult(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume) {
    i += consume;
    Parse::Expression* l = exp(col_names, s, i, 0);
    if (i < s.size() && s[i] == '*')
        return new BinOp {l, mult(col_names, s, i, 1), std::multiplies<double>() };
    if (i < s.size() && s[i] == '/')
        return new BinOp {l, mult(col_names, s, i, 1), std::divides<double>() };
    return l;
}


//sum = mult + sum | mult - sum | mult
Parse::Expression* sum(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume) {
    i += consume;
    Parse::Expression* l = mult(col_names, s, i, 0);
    if (i < s.size() && s[i] == '+')
        return new BinOp {l, sum(col_names, s, i, 1), std::plus<double>() };
    if (i < s.size() && s[i] == '-')
        return new BinOp {l, sum(col_names, s, i, 1), std::minus<double>() };
    return l;
}

//expr = sum
Parse::Expression* expr(const matrix::vector<std::string>& col_names, const std::string& s, size_t& i, unsigned consume) {
    i += consume;
    return sum(col_names, s, i, 0);
}

}

//*************************************
//  Implementation of the header file
//*************************************


matrix::size_type Parse::index(const matrix::vector<std::string>& col_names, const std::string& col_name) {
    return std::find(col_names.begin(), col_names.end(), col_name) - col_names.begin();
}

matrix::size_type Parse::get_index(const matrix::vector<std::string>& col_names, const std::string& col_name) {
    const auto POS = index(col_names, col_name);
    if (POS == col_names.size())
        throw Parse::col_name_not_found {col_name};
    return POS;
}

Parse::Parser::Parser(const matrix::vector<std::string>& col_names, const std::string& s) {
    size_t i {0};

    std::string s2;
    for(char c : s)
        if (!std::isspace(c))
            s2 += c;
    e = expr(col_names, s2, i, 0);
}

Parse::Parser::~Parser(){
    delete e;
}

double Parse::Parser::eval(size_t i, const matrix::vector<double>& row) {
    return e->eval(i, row);
}

bool Parse::Parser::name_clash(const std::string& name) {
    return (name.substr(0, 1) == "e" || name.substr(0, 2) == "pi" || 
        name.substr(0, 2) == "lg" || name.substr(0, 4) == "sqrt");
}

bool Parse::Parser::all_valid_chars(const std::string& name) { //Checks if all characters are valid.
    if (name.empty())
        return false;
    if (std::isdigit(name[0]))
        return false;
    for (char c : name)
        if (c != '_' && !isalnum(c))
            return false;
    return true;
}
bool Parse::Parser::valid_name(const std::string& name) {
    return !name_clash(name) && all_valid_chars(name);
}

void Parse::Parser::make_valid_names(matrix::vector<std::string>& col_names){
    for (matrix::size_type i = 0; i != col_names.size(); ++i) {
        if (Parse::Parser::name_clash(col_names[i]))
            col_names[i][0] = std::toupper(col_names[i][0]);
        if (!Parse::Parser::all_valid_chars(col_names[i]))
            col_names[i] = "COLUMN" + std::to_string(i);
    }
}