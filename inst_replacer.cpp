#include "inst_replacer.h"

Pattern_Search::Pattern_Search(std::vector<std::string> &source,std::vector<std::string> &pc){
    std::vector<std::string> split_operand;
    Instruction ins;
    for(int i = 0; i < source.size(); i++)
    {
            std::string tmp = source[i];
            if(tmp[0]=='.')
            {
                continue;
            }
            else
            {
                std::vector<std::string> tmp_line={"None","None","None","None"};
                int pos = 0;
                int pos2 = 0;
                int len = tmp.size();
                pos2 = tmp.find(' ', pos);
               
                if(pos2 == -1){
                    ins.op = tmp;
                }
                else
                {
                    ins.op = tmp.substr(pos, pos2 - pos);

                    ins.s_code =  source[i];
                    ins.pc = pc[i];
                    pos = pos2 + 1;
                    while (pos < len) {
                        pos2 = tmp.find(",", pos);
                        if(pos2 == -1) 
                            pos2 = len;
                        while(tmp[pos]==' ')
                            pos++;
                        if(tmp[pos]=='[')
                        {
                            if(tmp[pos2-1] != ']'){
                                while(tmp[pos2]!=']'){
                                    pos2++;
                                }
                                pos2++;
                            }          
                        }
                        ins.operand.push_back(tmp.substr(pos, pos2 - pos));
                        // std::cout<<tmp.substr(pos, pos2 - pos)<<std::endl;
                        pos = pos2 + 1;
                    }
                }
                split_code.push_back(ins);
                ins.clear();
            }
    }

        for(int i = 0; i < split_code.size(); i++){
                for(int k = 0; k < split_code[i].operand.size(); k++){
                    getOperand(split_operand, split_code[i].operand[k]);
                }
                split_code[i].operand = split_operand;
                split_operand.clear();
        }
    // for(int i = 0; i < split_code.size(); i++){
    //     for(int j = 0; j < split_code[i].operand.size();j++){
    //         std::cout<<split_code[i].operand[j]<<"   ";
    //     }
    // }

}


// void Pattern_Search::split_by_basicblock(std::vector<std::string> &source,std::vector<std::string> &pc,std::vector<std::string> &ins){
//     std::string codeline;
//     std::vector<std::string> code;
//     for(int i = 0; i <source.size();i++)
//     {
//         codeline = source[i];
//         int p1 = 0;
//         int p2 = codeline.size();
//         while(codeline[p1]=='\t') p1++;
//         while(codeline[p2]=='\t') p2--;
//         code.push_back(codeline.substr(p1, p2-p1));
//     }
    
//     for(int i = 0;i < code.size();i++)
//     {
//         int len = code[i].length();
        
//         if(code[i][len-1]==':'||is_branch(ins[i])||i==0)
//         {
//             std::vector<std::string> block;
//             std::vector<std::string> number;
//             std::vector<std::string> ins_b;
//             if(i==0) i--;
//             for(int j =i + 1;j<code.size();j++)
//             {
//                 len = code[j].size();
//                 if(code[j][len-1]==':')
//                 {
//                     basic_block.push_back(block);
//                     line_number.push_back(number);
//                     ins_block.push_back(ins_b);
//                     i=j-1;
//                     break;
//                 }
//                 else if(j==code.size()-1){
//                     block.push_back(code[j]);
//                     number.push_back(pc[j]);
//                     ins_b.push_back(ins[j]);
//                     basic_block.push_back(block);
//                     line_number.push_back(number);
//                     ins_block.push_back(ins_b);
//                     i=j-1;
//                     break;
//                 }
//                 else{
//                     if(code[j].size()>0)
//                     {
//                         block.push_back(code[j]);
//                         number.push_back(pc[j]);
//                         ins_b.push_back(ins[j]);
//                     }
                    
//                 }
                   
//             }
//             block.clear();
//             number.clear();
//             ins_b.clear();
//         }
//     }
//     //
// }
bool Pattern_Search::is_branch(std::string &s){
    if(s =="br"||s =="sub")
    {
    }
    return false;
}

int min(int a, int b){
    if(a>b)
        return b;
    else
        return a;
}
std::vector<Instruction> Pattern_Search::find_discontinuous_pattern(){
    std::vector<Instruction> res;
    int flag1=0;
    int flag2=0;
    int pos1, pos2;
    std::vector<std::string> write_ins = {"adc","adcs", "add", "adds","and", "ands", "eon", "eor", "sub","subs", "ldr", "mov"};
    std::vector<std::string> ii1,ii2,ii3;
    for(int i = 0; i <split_code.size(); ++i){
        flag1=0;
        flag2=0;
        if(split_code[i].op =="add"||split_code[i].op =="sub"||split_code[i].op =="or"||split_code[i].op =="and"||split_code[i].op =="xor")
        {   
            // std::cout<<"aa"<<std::endl;
            for(int j = i-1; j >= 0; --j)
            {
                if(split_code[j].op == "str"
                    ||(is_relate(split_code[i], split_code[j])&&split_code[j].op != "ldr")){
                    flag1 = 0;
                    break;
                }
                if(split_code[j].op=="ldr")
                {
                    if(is_relate(split_code[i], split_code[j]))
                    {
                        flag1 = 1;
                        pos1 = j;
                        break;
                    }
                    else
                    {
                        flag1 = 0;
                        break;
                    }
                }
                
            }       

            for(int j = i+1; j <split_code.size(); ++j)
            {
                if((is_relate(split_code[i], split_code[j])&&(split_code[j].op != "str"&&split_code[j].op!="ldr"))){
                    flag2 = 0;
                    break;
                }
                if(split_code[j].op=="str"||split_code[j].op=="ldr")
                {
                     
                    if(is_relate(split_code[i], split_code[j]))
                    {
                        flag2 = 1;
                        pos2 = j;
                        break;
                    }
                        
                    else
                    {
                        flag2=0;
                        break;
                    }
                }
                
            }
            if(flag1==1&&flag2==1)
            {
                
                bool flag3 = 1;
                bool flag4 = 0;
                bool flag_s1 = 0;
                std::string ch = split_code[pos1].operand[0];
                for(int j = pos2+1; j <min(pos2+20, split_code.size()); j++){
                    
                    // if(is_branch(split_code[j].bin)) break;
                    for(int k = 0; k < split_code[j].operand.size(); k++){
                        if(ch == split_code[j].operand[k]){
                            // if((split_code[j].op == "ldr"||split_code[j].op == "mov")&&ch == split_code[j].operand[0]){
                            if(find(write_ins.begin(), write_ins.end(),split_code[j].op) != write_ins.end()){
                                bool flag_t = 1;
                                for(int n = 1; n < split_code[j].operand.size(); n++){
                                     if(ch == split_code[j].operand[n]){
                                        flag_t = 0;
                                     }
                                }
                                if(flag_t == 1){
                                    flag_s1 = 1;
                                    flag4 = 1;
                                    break;
                                }
                                else{
                                    flag3 = 0;
                                    flag4 = 1;
                                    break;
                                }
                            }
                            else{
                                flag3 = 0;
                                flag4 = 1;
                                break;
                            }
                        }
                            
                    }
                    if(flag4)
                        break;
                    
                }

                bool flag5 = 1;
                bool flag6 = 0;
                bool flag_s2 = 0;
                std::string ch1 = split_code[i].operand[0];
                for(int j = pos2+1; j <min(pos2+20,split_code.size()); j++){
                    
                    // if(split_code[j].op[0] == 'b') break;
                    for(int k = 0; k < split_code[j].operand.size(); k++){
                        if(ch1 == split_code[j].operand[k]){
                            if(find(write_ins.begin(), write_ins.end(),split_code[j].op) != write_ins.end()){
                                bool flag_t = 1;
                                for(int n = 1; n < split_code[j].operand.size(); n++){
                                     if(ch1 == split_code[j].operand[n]){
                                        flag_t = 0;
                                     }
                                }
                                if(flag_t == 1){
                                    flag_s2 = 1;
                                    flag6 = 1;
                                    break;
                                }
                                else{
                                    flag5 = 0;
                                    flag6 = 1;
                                    break;
                                }
                            }
                            else{
                                flag5 = 0;
                                flag6 = 1;
                                break;
                            }
                        }
                            
                    }
                    if(flag6)
                        break;
                    
                }
                if(flag3==1&&flag5==1){
                    // std::cout<<flag_s1<<std::endl;
                    // std::cout<<flag_s2<<std::endl;
                    if(flag_s1 == 1&& flag_s2 == 1){
                        split_code[pos1].safe = 1;
                    }
                    res.push_back(split_code[pos1]);
                    res.push_back(split_code[i]);
                    res.push_back(split_code[pos2]);
                }
                
            }
            
        }
    }
    
    return res;

}

std::vector<Instruction> Pattern_Search::find_continuous_pattern(){
    std::vector<Instruction> res;
    int flag1=0;
    int flag2=0;
    int pos1, pos2;
    std::vector<std::string> ii1,ii2,ii3;
    for(int i = 0; i <split_code.size(); ++i){
        flag1=0;
        flag2=0;
        if(split_code[i].op =="add"||split_code[i].op =="sub"||split_code[i].op =="or"||split_code[i].op =="and"||split_code[i].op =="xor")
        {   
                if(i-1>=0 && split_code[i-1].op=="ldr")
                {
                    if(is_relate(split_code[i], split_code[i-1]))
                    {
                        flag1 = 1;
                        pos1 = i-1;
                    }
                    else
                    {
                        flag1 = 0;
                    }
                }
                    
                if(i+1<=split_code.size()-1&&split_code[i+1].op=="str")
                {
                     
                    if(is_relate(split_code[i], split_code[i+1]))
                    {
                        flag2 = 1;
                        pos2 = i+1;
                    }
                        
                    else
                    {
                        flag2=0;
                    }
                }
                
            if(flag1==1&&flag2==1)
            {
                
                bool flag3 = 1;
                bool flag4 = 0;
                std::string ch = split_code[pos1].operand[0];
                for(int j = pos2+1; j <split_code.size(); j++){
                    
                    if(split_code[j].op[0] == 'b') break;
                    for(int k = 0; k < split_code[j].operand.size(); k++){
                        if(ch == split_code[j].operand[k]){
                            
                            if((split_code[j].op == "ldr"||split_code[j].op == "mov")&&ch == split_code[j].operand[0]){
                                flag4 = 1;
                                break;
                            }
                            else{
                                flag3 = 0;
                                flag4 = 1;
                                break;
                            }
                        }
                            
                    }
                    if(flag4)
                        break;
                    
                }

                bool flag5 = 1;
                bool flag6 = 0;
                std::string ch1 = split_code[pos2].operand[0];
                for(int j = pos2+1; j <split_code.size(); j++){
                    
                    // if(split_code[j].op[0] == 'b') break;
                    for(int k = 0; k < split_code[j].operand.size(); k++){
                        if(ch1 == split_code[j].operand[k]){
                            
                            if((split_code[j].op == "ldr"||split_code[j].op == "mov")&&ch1 == split_code[j].operand[0]){
                                flag6 = 1;
                                break;
                            }
                            else{
                                flag5 = 0;
                                flag6 = 1;
                                break;
                            }
                        }
                            
                    }
                    if(flag6)
                        break;
                    
                }
                if(flag3==1&&flag5==1){
                    res.push_back(split_code[pos1]);
                    res.push_back(split_code[i]);
                    res.push_back(split_code[pos2]);
                }
                
            }
            
        }
    }

    
   
   
    return res;

}

bool Pattern_Search::is_relate(Instruction &i1, Instruction &i2)
{
    for(int i = 0; i< i1.operand.size();i++)
    {
        if(i1.operand[i]=="None")
        {
            continue;
        }
        else
        {
            std::string lr = i1.operand[i];
            for(int j = 0; j< i2.operand.size();j++)
            {
                 if(i2.operand[j]=="None"){
                    continue;
                }
                else{
                    std::string rr= i2.operand[j];
                    if(lr == rr){
                        return true;
                    }
                }   
            }  
        }   
    }
    return false;
}

void Pattern_Search::getOperand(std::vector<std::string> &str, std::string reg){
    int pos = 0;
    int pos2 = 0;
    int len = reg.size();
    if(reg[0] == '['){
        pos = 1;
        while (pos < len) {
            pos2 = reg.find(",", pos);
            if(pos2 == -1) 
                pos2 = len-1;
            if(reg[pos]==' ')
                pos++;
            str.push_back(reg.substr(pos, pos2 - pos));
            pos = pos2 + 1;
        }
    }
    else
        str.push_back(reg);
    
}

void Pattern_Search::operandCheck(std::vector<std::string> &str){
    for(int i = 0; i < str.size(); i++){
        if(str[i][0] == 'w')
            str[i][0] = 'x';
            
    }
}
