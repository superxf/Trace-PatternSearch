#include "inst_replacer.h"

using namespace std;
vector<vector<string>> f_pc;
vector<Instruction> l_a_s1;
vector<Instruction> l_a_s2;
vector<Instruction> l_a_t_s;
vector<Instruction> l_a_l;
int size1 = 0;
int size2 = 0;
int size3 = 0;
int size4 = 0;
int size1_s = 0;
int size2_s = 0;
int size3_s = 0;
int size4_s = 0;
int cnt = 0;
void type_check(Instruction& i1, Instruction& i2, Instruction& i3){
    if(i3.op =="str"){
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[0]){     
                    if(i1.operand[1] == i3.operand[1]){
                        size1++;
                        if(i1.operand.size()==i3.operand.size()&& i1.operand.size()==2||(i1.operand.size()==3 && i1.operand[2] == i3.operand[2])){
                            if(i2.operand.size()==3){
                                l_a_s1.emplace_back(i1);
                                l_a_s1.emplace_back(i2);
                                l_a_s1.emplace_back(i3);
                                if(i1.safe == 1)
                                    size1_s++;
                            }
                        }
                    }
                    else{
                        size2++;
                        if(i2.operand[2][0]!='#'&&i2.operand.size()==3){
                            if(((i1.operand.size()==3 || i3.operand.size() ==3)&&i2.operand[0]!=i2.operand[1]&&i2.operand[0]!=i2.operand[2])||(i1.operand.size()==2 && i3.operand.size() ==2)){
                                l_a_s2.emplace_back(i1);
                                l_a_s2.emplace_back(i2);
                                l_a_s2.emplace_back(i3);
                                if(i1.safe==1)
                                    size2_s++;
                            }
                        }
                    }
            }
        }
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[1]){
                size3++;
                if(i2.operand.size()==3&&i2.operand[2][0]!='#'){
                    if(((i1.operand.size()==3 || i3.operand.size() ==3)&&i1.operand[0]!=i2.operand[0])||(i1.operand.size()==2 && i3.operand.size() ==2)){
                            l_a_t_s.emplace_back(i1);
                            l_a_t_s.emplace_back(i2);
                            l_a_t_s.emplace_back(i3);
                            if(i1.safe==1)
                                    size3_s++;
                    }
                }
            }
        }
    }
    else{
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[1]){
                size4++;
                if(i2.operand.size()==3&&i2.operand[2][0]!='#'){
                    if(((i1.operand.size()==3 || i3.operand.size() ==3)&&i1.operand[0]!=i3.operand[0])||(i1.operand.size()==2 && i3.operand.size() ==2)){
                        l_a_l.emplace_back(i1);
                        l_a_l.emplace_back(i2);
                        l_a_l.emplace_back(i3);
                        if(i1.safe==1)
                                    size4_s++;
                    }
                }
            }
            
        }
    }
    
}

int main(int argc, char *argv[]){
    std::ifstream ifs;
    std::ofstream ofs;
    vector<string> block;
    vector<string> pc;
    vector<string> ins;
    ifs.open(argv[1],std::ios::in) ;
    if (!ifs.is_open()) {
        std::cout << "文件打开失败" << std::endl;
    }
    std::string codeline;
    int line_number = 0;
    int count = 0;
    vector<vector<Instruction>> tmp_res;
    string code;
    // vector<vector<Instruction>> tmp_res;
    while(getline(ifs, codeline)){
        // cout<<codeline<<endl;
        int pc_end = codeline.find(':');
        pc_end = codeline.find(':',pc_end+1);
        pc_end = codeline.find(':',pc_end+1);
        int ins_start = codeline.find(':',pc_end+1);
        int ins_end = codeline.find(':',ins_start+1);
        code = codeline.substr(ins_start+1,ins_end-ins_start-1);
        int p1 = 0;
        int p2 = code.size()-1;
        while(code[p1]==' ') p1++;
        while(code[p2]==' ') p2--;
        block.emplace_back(code.substr(p1, p2-p1+1));
        pc.emplace_back(codeline.substr(pc_end+4,6));
    }
    Pattern_Search search(block, pc);
    // std::cout<<"aaaa"<<std::endl;
    auto res = search.find_discontinuous_pattern();
    // cout<<res.size()<<endl;
    vector<Instruction> real_res;
    for(int i = 0; i < res.size(); i+=3){
        bool flag = 1;
        for(int j = 0; j < real_res.size(); j++){
            if(res[i].pc == real_res[j].pc){
                flag = 0;
                break;
            } 
        }
        if(flag == 1){
            real_res.push_back(res[i]);
            real_res.push_back(res[i+1]);
            real_res.push_back(res[i+2]);
        }
    }
    for(int j = 0; j < real_res.size(); j+=3){
        type_check(real_res[j], real_res[j+1], real_res[j+2]);
    }
    ifs.close();
    cout<<"padd1"<<endl;
    cout<<"所有满足条件的个数: "<<size1 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_s1.size()<<endl;
    cout<<"安全的个数: "<<size1_s * 3<<endl;
    cout<<endl;
    cout<<"padd2"<<endl;
    cout<<"所有满足条件的个数: "<<size2 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_s2.size()<<endl;
    cout<<"安全的个数: "<<size2_s * 3<<endl;
    cout<<endl;
    cout<<"plat"<<endl;
    cout<<"所有满足条件的个数: "<<size3 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_t_s.size()<<endl;
    cout<<"安全的个数: "<<size3_s * 3<<endl;
    cout<<endl;
    cout<<"pll"<<endl;
    cout<<"所有满足条件的个数: "<<size4 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_l.size()<<endl;
    cout<<"安全的个数: "<<size4_s * 3<<endl;

    ofs.open("res.txt", std::ios::out);
    ofs<<"padd1"<<" "<< l_a_s1.size()<<endl;
    for(int i = 0; i < l_a_s1.size(); i++){
        ofs<<l_a_s1[i].pc<<" " <<l_a_s1[i].bin<<" " <<l_a_s1[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"padd2"<<" "<< l_a_s2.size()<<endl;
    for(int i = 0; i < l_a_s2.size(); i++){
        ofs<<l_a_s2[i].pc<<" " <<l_a_s2[i].bin<<" " <<l_a_s2[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"plats"<<" "<< l_a_t_s.size()<<endl;
    for(int i = 0; i < l_a_t_s.size(); i++){
        ofs<<l_a_t_s[i].pc<<" " <<l_a_t_s[i].bin<<" " <<l_a_t_s[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"pll"<<" "<< l_a_l.size()<<endl;
    for(int i = 0; i < l_a_l.size(); i++){
        ofs<<l_a_l[i].pc<<" " <<l_a_l[i].bin<<" " <<l_a_l[i].s_code<<endl;
    }
    ofs.close();

    ofs.open("res_safe.txt", std::ios::out);
    ofs<<"padd1"<<" "<< size1_s<<endl;
    for(int i = 0; i < l_a_s1.size(); i+=3){
        if(l_a_s1[i].safe == 1){
            ofs<<l_a_s1[i].pc<<" " <<l_a_s1[i].bin<<" " <<l_a_s1[i].s_code<<endl;
            ofs<<l_a_s1[i+1].pc<<" " <<l_a_s1[i+1].bin<<" " <<l_a_s1[i+1].s_code<<endl;
            ofs<<l_a_s1[i+2].pc<<" " <<l_a_s1[i+2].bin<<" " <<l_a_s1[i+2].s_code<<endl;
        }
        
    }
    ofs<<endl;
    ofs<<"padd2"<<" "<< size2_s<<endl;
    for(int i = 0; i < l_a_s2.size(); i++){
        if(l_a_s2[i].safe==1){
            ofs<<l_a_s2[i].pc<<" " <<l_a_s2[i].bin<<" " <<l_a_s2[i].s_code<<endl;
            ofs<<l_a_s2[i+1].pc<<" " <<l_a_s2[i+1].bin<<" " <<l_a_s2[i+1].s_code<<endl;
            ofs<<l_a_s2[i+2].pc<<" " <<l_a_s2[i+2].bin<<" " <<l_a_s2[i+2].s_code<<endl;
        }
        
    }
    ofs<<endl;
    ofs<<"plats"<<" "<< size3_s<<endl;
    for(int i = 0; i < l_a_t_s.size(); i++){
        if(l_a_t_s[i].safe==1){
            ofs<<l_a_t_s[i].pc<<" " <<l_a_t_s[i].bin<<" " <<l_a_t_s[i].s_code<<endl;
            ofs<<l_a_t_s[i+1].pc<<" " <<l_a_t_s[i+1].bin<<" " <<l_a_t_s[i+1].s_code<<endl;
            ofs<<l_a_t_s[i+2].pc<<" " <<l_a_t_s[i+2].bin<<" " <<l_a_t_s[i+2].s_code<<endl;
        }
        
    }
    ofs<<endl;
    ofs<<"pll"<<" "<< size4_s<<endl;
    for(int i = 0; i < l_a_l.size(); i++){
        if(l_a_l[i].safe == 1){
            ofs<<l_a_l[i].pc<<" " <<l_a_l[i].bin<<" " <<l_a_l[i].s_code<<endl;
            ofs<<l_a_l[i+1].pc<<" " <<l_a_l[i+1].bin<<" " <<l_a_l[i+1].s_code<<endl;
            ofs<<l_a_l[i+2].pc<<" " <<l_a_l[i+2].bin<<" " <<l_a_l[i+2].s_code<<endl;
        }
        
    }
    ofs.close();
}