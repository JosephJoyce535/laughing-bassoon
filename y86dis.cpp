#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdlib>

std::string detR(std::string);
std::string detOC(std::string);
std::string addy(std::string val);
std::string trailingzero(std:: string val);

/* @author Kyle Joyce
 * @author Jacob Mazzarese
 */

int main(int argc, char *argv[]) {
    std::string file = "";
    file += argv[1];
    std::ifstream inf(file);
    if (!inf) {
        std::cerr << "Error: File couldn't be opened for reading!";
        exit(1);
    }
    std::string strInput;

    //reads in file line by line
    while (getline(inf, strInput)) {
        std::string str = strInput.substr(0,6); 
        size_t pos = strInput.find(" ");
        std::string str2 = strInput.substr(pos+1);
        size_t pose = str2.find(" ");
        int len = pose - pos + 6;

        //filters if the line is length of a mnemoic 
        if (len == 2 || len == 4 || len == 18 || len == 20) {
            std::string opcode = str2.substr(0,2);
            std::string op = str2.substr(0,1);
            std:: string add;
            if (op == "2" || op == "4" || op == "5" || op == "6") {
                std::string registerA = str2.substr(2,1);
                std::string registerB = str2.substr(3,1);
                if (op == "2" || op == "6") {
                    std::cout << str << "  " << detOC(opcode) << "  " << detR(registerA) << ", " << detR(registerB) << "\n";
                }
                else if (op == "4") {
                    add = strInput.substr(11);
                    std::cout << str << "  " << detOC(opcode) << "  " << detR(registerA) << ", " << "$0x" <<  trailingzero(addy(add)) << " (" << detR(registerB) << ")\n";
                }
                else {
                    add = strInput.substr(11);
                    std::cout << str << "  " << detOC(opcode) << " $0x" << trailingzero(addy(add)) << "(" << detR(registerB) << "), " << detR(registerA) << "\n";
                }
            }
            else if (op == "3") {
                add = strInput.substr(11);
                std::string registerB = str2.substr(3,1);
                std::cout << str << "  " << detOC(opcode) << "  $0x" << trailingzero(addy(add)) << ",  " << detR(registerB) << "\n";
            }
            else if (op == "A" || op == "B" || op == "a" || op == "b") {
                std::string registerA = str2.substr(2,1);
                std::cout << str << "  " << detOC(opcode) << "  " << detR(registerA) << "\n";
            }
            else if (op == "7" || op == "8") {
                add = strInput.substr(9);
                std::cout << str << "\t" << detOC(opcode) << "  0x" <<  trailingzero(addy(add)) << "\n";
            }
            else {
                std::cout << str << "  " << detOC(opcode) << "\n";
            }
        }
        //catches quad directives
        else if (len == 16) {
            std::string quad = strInput.substr(7);
            std::cout << str << " .quad  0x" << addy(quad) << "\n";
        }
        //prints just the memory address if no instruction or quad
        else {
            std::cout << str << "\n";
        }
    }
    return 0;
}
/*
 * This function converts a hex string address from little
 * endian to big endian
 */
std::string addy(std::string val)
{
    int e = 15;
    int j = 14;
    char A[16];
    for(int i = 0; i < 15; i += 2)
    {
        A[j] = val[i];
        j -= 2;
    }
    for(int f = 1; f < 16; f += 2)
    {
        A[e] = val[f];
        e -= 2;
    }
    for(int r = 0; r < 16; r++)
    {
        val[r] = A[r];
    }
    return val;
}

/*
 * Takes a hex string useless zero values until first non zero 
 * integer was read
 */
std:: string trailingzero(std:: string val)
{
    int det = 0;
    for(int i = 0; i < 16; i++)
    {
        if(val[i] != '0')
        {
            det++;
        }
    }
    if (det == 0)
    {
        return "0";
    }
    while(val[0] == '0')
    {
        val.erase(0,1);
    }
           
    return val;
}

//takes a substring for bit associated with a register and returns string of corresponding register name
std::string detR(std::string rA) {
   if (rA == "0") {
     return "%rax";
   }
   else if (rA == "1") {
      return "%rcx";
   }
   else if (rA == "2") {
        return "%rdx";
   }
   else if (rA == "3") {
        return "%rbx";
   }
   else if (rA == "4") {
        return "%rsp";
   }
   else if (rA == "5") {
        return "%rbp";
   }
   else if (rA == "6") {
        return "%rsi";
   }
   else if (rA == "7") {
       return "%rdi";
   }
   else if (rA == "8") {
        return "%r8";
   }
   else if (rA == "9") {
        return "%r9";
   }
   else if (rA == "a") {
        return "%r10";
   }
   else if (rA == "b") {
        return "%r11";
   }
   else if (rA == "c") {
        return "%r12";
   }
   else if (rA == "d") {
        return "%r13";
   }
   else if (rA == "e") {
        return "%r14";
   }
   else if (rA == "f") {
        return "NOT A REGISTER";
   }
   else {
        return "ERROR OCCURED";
   }
}

//takes a substring for bits corresponding with opcode in a mnemoic and return the string for that opcode
std::string detOC(std::string opcode) {
   std::string first = opcode.substr(0,1);
   std::string second = opcode.substr(1,1);
   if (first == "0") {
      return "halt";
   }
   else if (first == "1") {
      return "nop";
   }
   else if (first == "2") {
       if (second == "0") {
            return "rrmovq";
       }
       else if (second == "1") {
            return "cmovle";
       }
       else if (second == "2") {
            return "cmov1";
       }
       else if (second == "3") {
            return "cmove";
       }
       else if (second == "4") {
            return "cmovne";
       }
       else if (second == "5") {
            return "cmovge";
       }
       else if (second == "6") {
            return "cmovg";
       }
       else {
           // error occured
           return "illegalop";
       }
   }
   else if (first == "3") {
        return "irmovq";
   }
   else if (first == "4") {
        return "rmmovq";
   }
   else if (first == "5") {
        return "mrmovq";
   }
   else if (first == "6") {
       if (second == "0") {
            return "addq";
       }
       else if (second == "1") {
            return "subq";
       }
       else if (second == "2") {
            return "andq";
       }
       else if (second == "3") {
            return "xorq";
       }
       else {
          // error occured
          return "illegalop";
       }
   }
   else if (first == "7") {
        if (second == "0") {
            return "jmp";
        }
        else if (second == "1") {
            return "jle";
        }
        else if (second == "2") {
            return "jl";
        }
        else if (second == "3") {
            return "je";
        }
        else if (second == "4") {
            return "jne";
        }
        else if (second == "5") {
            return "jge";
        }
        else if (second == "6") {
            return "jg";
        }
        else {
            // error occured
            return "illegalop";                                                                                           
        }
   }
   else if (first == "8") {
        return "call";
   }
   else if (first == "9") {
        return "ret";
   }
   else if (first == "A" || first == "a") {
        return "pushq";
   }
   else if (first == "B" || first == "b") {
        return "popq";
   }
   else {
    //error occured
    return "illegalop";
   }
}
