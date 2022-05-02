#include "helper.h" //For vector and matrix::size_type. 
#include <exception>

/*
The grammar is as follows:

expr = sum
sum = mult + sum | mult - sum | mult
mult = exp * mult | exp / mult | exp
exp = term ^ exp | term
term = ( expr ) | pi | e | $i | 
    lg term | sqrt term | - term
    [a-Z|_][a-Z|0-9|_]* | [0-9]+(.[0-9]+)?

Some remarks:
1. [A-Z|_][A-Z|0-9|_]* means all the letters or an underscore followed by 0 or more all letters or digits or underscore. So 4hour is not valid but _4hour is. 
   These names refer to a column name and will be converted to the corresponding index.
2. lg stands for logorithm, will use std::log, which returns the natural logorithm.
3. pi and e are reserved constants which will be parsed into their corresponding values.
4. $i is a special sign followed by i, which stands for index. This will evaluate to the index of the row. So "$i" will simply number each row.
5. lgAge will be interpeted as a column name. lg Age will be interpreted as lg(Age)
6. Throwing errors messes with the tree structure of pointers (? Not sure about this). On failure in the building threes we therefore do not throw errors.
   We return a nullptr and append the errorstring. The Parser will then throw an error.
7. (IMPORTANT): This grammar is not completely functional. 1 - 3 - 1 gets parsed as 1-(3-1) 
   This happens because sum = mult - sum. This evaluates to -1 instead of -3 as we might expect.
   ***This can be fixed by using while loops rather than recursive calls***
*/

namespace Parse {
    struct parse_error : std::runtime_error { parse_error(const std::string& msg) :std::runtime_error(msg) {}};

    struct col_name_not_found : parse_error { col_name_not_found(const std::string& msg) : parse_error{"Column name not found: " + msg} {}};
    struct invalid_col_name : parse_error { invalid_col_name(const std::string& msg) : parse_error{"Invalid column name: " + msg} {}};


    matrix::size_type index(const matrix::vector<std::string>& col_names, const std::string& col_name); //Returns col_names.size() on failure
    matrix::size_type get_index(const matrix::vector<std::string>& col_names, const std::string& col_name); //Raises an exception on failure

    struct Expression {
        virtual ~Expression() {}
        virtual double eval(matrix::size_type i, const matrix::vector<double>& row) const =0;
    };

    class Parser {
    public:
        Parser(const matrix::vector<std::string>& col_names, const std::string& s);
        ~Parser();

        double eval(size_t i, const matrix::vector<double>& row);
        static bool name_clash(const std::string& name); //Checks for reserved terms such as 'pi' or 'e'.
        static bool valid_letter(char c);
        static bool all_valid_chars(const std::string& name); //Checks if all characters are valid. Returns false on empty string.
        static bool valid_name(const std::string& name); 
        static void make_valid_names(matrix::vector<std::string>& col_names); //Changes column names if necessary. Nameclashes are resolved by capitalizing. Otherwise the ith column is renamed to "COLUMNi"

        //The parser is intended for only a single use.
        Parser(const Parser&) = delete;
        Parser(Parser&&) = delete;
        Parser& operator=(const Parser&) = delete;
        Parser& operator=(Parser&&) = delete;
    private:   
        Expression* e;
    };

}