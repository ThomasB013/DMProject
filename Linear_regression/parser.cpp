#include "parser.h"

#include <functional>
#include <algorithm>
#include <math.h>
#include <sstream>


namespace { //Unnamed namespace

struct Bin_Op : Parse::Expression { //Binary operations
    Bin_Op(Expression* left, Expression* right, std::function<double(double, double)> f) : l{left}, r{right}, op{f} {}
    ~Bin_Op() { delete l; delete r; }
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return op(l->eval(i, row), r->eval(i, row)); }
    Expression* l, * r;
    std::function<double(double, double)> op;
};

struct Un_Op : Parse::Expression { //Unary operations
    Un_Op(Expression* exp, std::function<double(double)> f) :e {exp}, op {f} {}
    ~Un_Op() { delete e; }
    double eval(matrix::size_type i, const matrix::vector<double>& row) const { return op(e->eval(i, row)); }
    Expression* e;
    std::function<double(double)> op;
};

struct Look_Up : Parse::Expression { //Look up the value in the row
    Look_Up(matrix::size_type i) : index {i} {}
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

enum class Kind {
    Plus, Minus, Mult, Div, Exp,
    Br_Open, Br_Close, Col_Index, Row_I,
    Math_Pi, Math_E, Math_Lg, Math_Sqrt,
    Numeric, End
};

struct Token {
    Token(Kind k) :kind(k) {}
    Token(Kind k, double v) :kind{k}, value{v} {}
    Token(Kind k, size_t i) :kind{k}, index{i} {}
    Kind kind;
    union {
        double value;
        size_t index;
    };
};

class Token_Stream {
public:
    Token_Stream(const std::string& expr_input, const matrix::vector<std::string>& col_names) {
        std::stringstream stream {expr_input}; //Read from this.

        for (char c; stream.get(c); ){
            if (isspace(c)) 
                continue;
            
            if (isdigit(c)) {
                stream.putback(c);
                double v;
                stream >> v;
                tokens.push_back({Kind::Numeric, (double) v});
            }
            else if (Parse::Parser::valid_letter(c)) {
                std::string s;
                for (s += c; stream.get(c) && Parse::Parser::valid_letter(c); )
                    s+= c;
                if (!stream.eof()) //Meaning we stopped reading because we encountered another character. The putback() is said to ignore the eofbit on cplusplus.com.
                    stream.putback(c);

                if (s == "pi")
                    tokens.push_back({Kind::Math_Pi});
                else if (s == "e")
                    tokens.push_back({Kind::Math_E});
                else if (s == "lg")
                    tokens.push_back({Kind::Math_Lg});
                else if (s == "sqrt")
                    tokens.push_back({Kind::Math_Sqrt});
                else 
                    tokens.push_back({Kind::Col_Index, (size_t) Parse::get_index(col_names, s)});
            }
            else if (c == '$') {
                if (stream.get(c) && c == 'i')
                    tokens.push_back({Kind::Row_I});
                else 
                    throw Parse::parse_error {"Epected i after $"};
            }
            else if (c == '(')
                tokens.push_back({Kind::Br_Open});
            else if (c == ')')
                tokens.push_back({Kind::Br_Close});
            else if (c == '-')
                tokens.push_back({Kind::Minus});
            else if (c == '+')
                tokens.push_back({Kind::Plus});
            else if (c == '*')
                tokens.push_back({Kind::Mult});
            else if (c == '/')
                tokens.push_back({Kind::Div});
            else if (c == '^')
                tokens.push_back({Kind::Exp});
            else 
                throw Parse::parse_error {"Unexpected character: " + c};       
        }
        tokens.push_back({Kind::End});
    }

    //Increments the token pointer to the next one.
    void consume() {
        ++i;
        //I choose to have an end token in the vector rather than returning it so that
        //If something gows wrong (for example we try to read out of bounds) we get an error message rather than
        //Always returning Kind::End after reading out of bounds.
        if (i == tokens.size())
            throw Parse::parse_error{"Token stream exhausted"};
    }

    //Returns the current token.
    Token current() const {
        if (i == tokens.size())
            throw Parse::parse_error{"Token stream exhausted"};
        return tokens[i];

    }
private:
    matrix::size_type i { 0 };
    matrix::vector<Token> tokens;
};



Parse::Expression* expr(Token_Stream& ts, bool consume, std::string& err_msg);

/*term = ( + expr + ) | e | pi | $i | 
    lg term | sqrt term | -term
    [a-Z|_][a-Z|0-9|_]* | [0-9]+(.[0-9]+)? */

Parse::Expression* term(Token_Stream& ts, bool consume, std::string& err_msg) { 
    if (consume)
        ts.consume();
    
    Token t = ts.current();
    if (t.kind == Kind::End) {
        err_msg += "Expected a term.";
        return nullptr;
    }
    ts.consume(); //For the next we always need to consume.
    switch (t.kind){
        case Kind::Br_Open: {
            Parse::Expression *e = expr(ts, false, err_msg);
            if (ts.current().kind == Kind::Br_Close){
                ts.consume();
                return e;
            }
            else {
                err_msg += "Expected closing brackets.";
                delete e;
                return nullptr;
            }
        }
        case Kind::Math_E:
            return new Val { Helper::MATH_E };
        case Kind::Math_Pi:
            return new Val { Helper::MATH_PI };
        case Kind::Row_I:
            return new Index {};
        case Kind::Col_Index:
            return new Look_Up { t.index };
        case Kind::Math_Lg:
            return new Un_Op {term(ts, false, err_msg), [](double x) -> double { return std::log(x); } };
        case Kind::Math_Sqrt:
            return new Un_Op {term(ts, false, err_msg), [](double x) -> double { return std::sqrt(x); } };
        case Kind::Minus:
            return new Un_Op {term(ts, false, err_msg), std::negate<double>() };
        case Kind::Numeric:
            return new Val { t.value };
        default: {
            err_msg += "Uncaught token in term parser: " + std::to_string(static_cast<int>(t.kind));
            return nullptr;
        }
    }
}

//exp = term ^ exp | term
Parse::Expression* exp(Token_Stream& ts, bool consume, std::string& err_msg) {
    if (consume)
        ts.consume();
    Parse::Expression* t = term(ts, false, err_msg);
    if (ts.current().kind == Kind::Exp)
        return new Bin_Op {t, exp(ts, true, err_msg), [](double x, double n) -> double { return std::pow(x, n); } };
    return t;
}

//mult = exp * mult | exp / mult | exp
Parse::Expression* mult(Token_Stream& ts, bool consume, std::string& err_msg) {
    if (consume)
        ts.consume();
    Parse::Expression* l = exp(ts, false, err_msg);
    switch(ts.current().kind){
        case Kind::Mult:
            return new Bin_Op {l, mult(ts, true, err_msg), std::multiplies<double>() };
        case Kind::Div:
            return new Bin_Op {l, mult(ts, true, err_msg), std::divides<double>() };
        default:
            return l;
    }
}

//sum = mult + sum | mult - sum | mult
Parse::Expression* sum(Token_Stream& ts, bool consume, std::string& err_msg) {
    if (consume)
        ts.consume();
    
    Parse::Expression* l = mult(ts, false, err_msg);
    switch(ts.current().kind){
        case Kind::Plus:
            return new Bin_Op {l, sum(ts, true, err_msg), std::plus<double>() }; 
        case Kind::Minus:
            return new Bin_Op {l, sum(ts, true, err_msg), std::minus<double>() };
        default:
            return l;
    }
}

//expr = sum
Parse::Expression* expr(Token_Stream& ts, bool consume, std::string& err_msg) {
    if (consume)
        ts.consume();    
    return sum(ts, false, err_msg);
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
    Token_Stream ts(s, col_names);
    std::string err_msg;
    e = expr(ts, false, err_msg);

    if (!err_msg.empty()) {
        delete e;
        e = nullptr;
        throw parse_error {err_msg};
    }
}

Parse::Parser::~Parser(){
    delete e;
}

double Parse::Parser::eval(size_t i, const matrix::vector<double>& row) {
    return e->eval(i, row);
}

bool Parse::Parser::name_clash(const std::string& name) {
    return (name == "e" || name == "pi" || 
        name == "lg" || name == "sqrt");
}

bool Parse::Parser::valid_letter(char c) {
    return std::isalnum(c) || c == '_';
}

bool Parse::Parser::all_valid_chars(const std::string& name) { //Checks if all characters are valid.
    if (name.empty())
        return false;
    if (std::isdigit(name[0]))
        return false;
    for (char c : name)
        if (!valid_letter(c))
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