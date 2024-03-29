#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <tuple>
#include <algorithm>

class Pattern{
    public:
    std::vector<std::string> op_list;
    std::vector<std::vector<int>> relation;
};

class Instruction{
    public:
    std::string op;
    std::vector<std::string> operand;
    int pos;
    std::string pc;
    std::string s_code;
    std::string bin;
    bool safe = 0;
    void clear(){
        operand.clear();
        op.clear();
    }
};


class Pattern_Search{
    public:
    Pattern_Search(std::vector<std::string> &source,std::vector<std::string> &pc);
    ~Pattern_Search() {}
    // void split_by_basicblock(std::vector<std::string> &source,std::vector<std::string> &pc,std::vector<std::string> &ins);
    bool confict();
    std::vector<Instruction> find_discontinuous_pattern();
    std::vector<Instruction> find_continuous_pattern();
    bool is_branch(std::string &s);
    bool is_relate(Instruction &i1, Instruction &i2);
    void getOperand(std::vector<std::string> &str, std::string reg);
    void operandCheck(std::vector<std::string> &str);


    private:
    std::vector<std::vector<std::string>> operandList;
    std::vector<std::vector<std::string>> basic_block;
    std::vector<std::vector<std::string>> line_number;
    std::vector<std::vector<std::string>> ins_block;
    std::vector<Instruction> split_code;
};
