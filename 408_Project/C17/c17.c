//sprintf是 C 标准库中的函数，用于格式化字符串，但​​它不会检查目标缓冲区的长度​​。如果写入的数据超出缓冲区大小，会导致​​缓冲区溢出​​（严重的安全漏洞，可能引发程序崩溃或被攻击）。

// #include <unistd.h> linux的，不兼容win的文件操作

#include <fcntl.h>   // 文件操作符文件操作符，定义 O_WRONLY, O_CREAT
#include <io.h>      // 提供 _open(), _close(), _write()
#include <sys/stat.h> // _S_IREAD, _S_IWRITE
#include <share.h>   // 提供 _SH_DENYNO
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

char* data; // 数据段 ，指向一个字节
uint64_t* stack; // 栈段

// 寄存器
uint64_t* pc;
uint64_t* sp; // 指向栈顶的地址
uint64_t* bp;
uint64_t ax;

///////////////////////////int-->uint64_t///////////////////
/////////////////  C17   ////////////////////////
/////////////////////////C17不支持 += -=这样的操作/////////////

uint64_t* code; // 代码段
uint64_t* code_dump; // 指向代码段的副本，地址是个int类型
uint64_t code_dump_1;

char* src;
char *src_dump;

uint64_t MAX_SIZE; // 定义内存段（代码段、数据段、栈）的最大容量
uint64_t cycle; // 指令计时器; 可以限制vm最大执行步数

char *src;
char *src_dump;

uint64_t line = 1;

uint64_t token;
uint64_t token_val;

uint64_t* symbol_table;
uint64_t* symbol_ptr;

uint64_t* main_ptr;

// symbol table
enum { Token, Hash, Name, Class, Type, Value, GClass, GType, GValue, SymSize };

// class & token
enum {
    Num = 128, Fun, Sys, Glo, Loc, Id,
    Char, Int, Enum, If, Else, Return, Sizeof, While,
    Assign, Cond, Lor, Land, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge,
    Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

// vm instruction set
enum {
    IMM, LEA, JMP, JZ, JNZ, CALL, NVAR, DARG, RET, LI, LC, SI, SC, PUSH,
    OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
    OPEN, READ, CLOS, PRTF, MALC, FREE, MSET, MCMP, EXIT
};

// type 
enum { CHAR, INT, PTR }; // INT指的是uint64_t，因为要实现自举

void tokenize() // 提取源代码的token, token_val；创建符号表
{
    // 暂理指针
    char* tmp_ptr;

    while ((token = *src++))
    {
        // 处理换行
        if (token == '\n') { line++; }

        // 处理宏
        else if (token == '#') { while (*src != 0 && *src != '\n') { src++; } }

        // 处理符号
            // 提取标识符
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
        {
            tmp_ptr = src - 1;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || *src == '_' || (*src >= '0' && *src <= '9'))
            {
                // 单次hash
                token = token * 1729 + *src++;
            }

            // 总的hash
            token = (token << 6) + (src - tmp_ptr); // 加上标识符的字符长度。无需+1，因为src指向末尾的下一个字符了

            // 在符号表中寻找相同符号
            symbol_ptr = symbol_table;

            while (symbol_ptr[Token] != 0) // 为0就是新的
            {
                if ((token == symbol_ptr[Hash]) && !memcmp(symbol_ptr[Name], tmp_ptr, src - tmp_ptr))
                {
                    token = symbol_ptr[Token];
                    return;
                }
                symbol_ptr += SymSize;
            }

            // 添加新的符号
            symbol_ptr[Name] = (uint64_t)tmp_ptr;
            symbol_ptr[Hash] = token;
            symbol_ptr[Token] = Id;
            token = Id;
            return;
        }

        // 处理数字
        else if (token >= '0' && token <= '9')
        {
            // 十进制
            if ((token_val = token - '0')) //已经通过token获得最高位，下面使用src就行
            {
                while (*src >= '0' && *src <= '9')
                {
                    token_val = token_val * 10 + *src++ - '0';
                }
            }

            // 十六进制
            else if ((*src == 'x') || (*src == 'X'))
            {
                src++;
                while ((*src >= '0' && *src <= '9') || (*src >= 'a' && *src <= 'f') || (*src >= 'A' && *src <= 'F'))
                {
                    if (*src >= '0' && *src <= '9') { token_val = token_val * 16 + *src++ - '0'; }
                    else if (*src >= 'a' && *src <= 'f') { token_val = token_val * 16 + (10 + *src++ - 'a'); }
                    else { token_val = token_val * 16 + (10 + *src++ - 'A'); }
                }
            }

            // 八进制
            else
            {
                while (*src >= '0' && *src <= '7')
                {
                    token_val = token_val * 8 + *src++ - '0';
                }
            }

            token = Num;
            return;
        }

        // 处理 char/string
        else if (token == '"' || token == '\'')
        {
            tmp_ptr = data; // 记录起始地址，存入data区
            while (*src != 0 && *src != token)
            {
                token_val = *src++;
                if (token_val == '\\') // 这里不是换行符，而是\n，无特殊用途
                {
                    token_val = *src++;
                    if (token_val == 'n') { token_val = '\n'; }
                    if (token_val == 't') { token_val = '\t'; }
                    if (token_val == '?') { token_val = '\?'; } // char str1[] = "This is a \\?/ form feed";

                }

                if (token == '"') { *data++ = token_val; }
            }
            src++;
            if (token == '"')
            {
                token_val = (uint64_t)tmp_ptr;
            }
            else { token = Num; } // 字符  token_val之前已经返回了字符的值！

            return;
        }

        // 处理注释和除法
        else if (token == '/')
        {
            if (*src = '/')
            {
                while (*src != 0 && *src != '\n') { src++; }
            }
            else
            {
                token = Div;
            }

            return;
        }

        // 处理operators(关键字)  
        else if (token == '=') { if (*src == '=') { src++; token = Eq; } else { token = Assign; } return; }
        else if (token == '+') { if (*src == '+') { src++; token = Inc; } else { token = Add; } return; }
        else if (token == '-') { if (*src == '-') { src++; token = Dec; } else { token = Sub; } return; }
        else if (token == '!' && *src == '=') { src++; token = Ne; return; } //逻辑非我们将在代码里实现，不用指令
        else if (token == '<') {
            if (*src == '=') { src++; token = Le; }
            else if (*src == '<') { src++; token = Shl; }
            else { token = Lt; } return;
        }
        else if (token == '>') {
            if (*src == '=') { src++; token = Ge; }
            else if (*src == '>') { src++; token = Shr; }
            else { token = Gt; } return;
        }
        else if (token == '|') { if (*src == '|') { src++; token = Lor; } else { token = Or; } return; }
        else if (token == '&') { if (*src == '&') { src++; token = Land; } else { token = And; } return; }
        else if (token == '^') { token = Xor; return; }
        else if (token == '%') { token = Mod; return; }
        else if (token == '*') { token = Mul; return; }
        else if (token == '[') { token = Brak; return; }
        else if (token == '?') { token = Cond; return; } // 三元组
        else if (token == '~' || token == ';' || token == '{' || token == '}'
            || token == '(' || token == ')' || token == ']' || token == ',' || token == ':') {
            return;
        } // 暗含空格跳过
    }
}

void assert(uint64_t tk)
{
    if (token != tk)
    {
        printf("line %lu: expect token: %lu(%c), get: %lu(%c)\n", line, tk, (char)tk, token, (char)token);
        exit(-1);
    }
    tokenize();
}

void check_new_id()
{
    if (token != Id)
    {
        printf("line %lu: invalid identifer\n", line);
        exit(-1);
    }
    if (symbol_ptr[Class] != 0)
    {
        printf("line %lu: duplicate declaration\n", line);
        exit(-1);
    }
}

void parse_enum()
{
    int i = 0; // 枚举元素的值,后期可以是负数，所以用int

    while (token != '}')
    {
        check_new_id();
        assert(Id);

        // 是不是有特殊赋值
        if (token == Assign)
        {
            assert(Assign);
            assert(Num);
            i = token_val;
        }
        symbol_ptr[Class] = Num;
        symbol_ptr[Type] = INT;
        symbol_ptr[Value] = i++;

        if (token == ',') tokenize();
    }
}

uint64_t parse_base_type()
{
    if (token == Int) { assert(Int); return INT; }
    else { assert(Char); return CHAR; }
}

void check_local_id() // 在同一作用域中，检查是不是一个之前有的local Id
{
    if (token != Id)
    {
        printf("line %lu: invalid identifer\n", line);
        exit(-1);
    }
    if (symbol_ptr[Class] == Loc)
    {
        printf("line %lu: duplicate declaration\n", line);
        exit(-1);
    }
}

// hide = 1为隐藏，0为恢复隐匿
void hide_and_recover_global(uint64_t hide)
{
    if (hide == 1)
    {
        symbol_ptr[GClass] = symbol_ptr[Class];
        symbol_ptr[GValue] = symbol_ptr[Value];
        symbol_ptr[GType] = symbol_ptr[Type];
    }
    else if (hide == 0)// hide = 0
    {
        symbol_ptr[Class] = symbol_ptr[GClass];
        symbol_ptr[Value] = symbol_ptr[GValue];
        symbol_ptr[Type] = symbol_ptr[GType];
    }
    else
    {
        printf("fail to hide or recover global variables!\n");
        exit(-1);
    }
}

uint64_t ibp; // 当前函数栈帧的基地址
void parse_param()
{
    uint64_t type, i;
    i = 0;

    while (token != ')')
    {
        type = parse_base_type();
        while (token == '*')
        {
            assert(Mul);
            type += PTR;
        }

        check_local_id();
        hide_and_recover_global(1); // 隐匿全局变量

        symbol_ptr[Class] = Loc;
        symbol_ptr[Type] = type;
        symbol_ptr[Value] = i++; // 表示第几个参数

        assert(Id);
        if (token == ',') assert(',');
    }

    ibp = ++i; // 参数个数 + 1
}

// 一个函数写了400多行，哈哈哈
uint64_t type;
void parse_expression(uint64_t precedence) // precedence是爬山的优先级阈值，token优先级必须大于它才能递归。这也符合优先级越大越要优先处理，所以要递归下降。到时候最后得到的函数栈（优先级最高）最先处理
{
    uint64_t tmp_type, i;
    uint64_t* tmp_ptr;

    if (token == Num) // 数字/字符
    {
        *++code = IMM;
        *++code = token_val;
        type = INT; // 把字符看作数字(ASCII码)
        assert(Num);
    }
    else if (token == '"') // 字符串
    {
        *++code = IMM;
        *++code = token_val; // 字符串地址
        assert('"');
        while (token == '"') { assert('"'); } // 多行字符串
        data = (char*)(((uint64_t)data + 8) & -8); // 字节对齐
        type = PTR;
    }
    else if (token == Sizeof)
    {
        assert(Sizeof);
        assert('(');
        type = parse_base_type();
        while (token == Mul) // 可以解析这种类型的指针：sizeof(int ***)
        {
            type += PTR;
            assert(Mul);
        }
        assert(')');
        *++code = IMM;
        *++code = (type == CHAR) ? 1 : 8; // 包含指针，uint64_t类型
        type = INT;
    }
    else if (token == Id) // 变量+函数
    {
        assert(Id);
        tmp_ptr = symbol_ptr;  // 因为递归调用，保存指向内容

        if (token == '(') // 函数调用，water("He", "She", 2);
        {
            assert('(');
            i = 0; // 记录函数参数个数
            while (token != ')') // 处理参数
            {
                parse_expression(Assign);
                *++code = PUSH;
                ++i;
                if (token == ',') assert(','); // 处理多个参数
            }
            assert(')');

            if (tmp_ptr[Class] == Sys) // 系统调用+函数都是函数调用
            {
                *++code = tmp_ptr[Value]; // 系统调用函数的Value是他的名称
            }
            else if (tmp_ptr[Class] == Fun)
            {
                *++code = CALL;
                *++code = tmp_ptr[Value];
            }
            else
            {
                printf("line %lu: invalid function call\n", line);
                exit(-1);
            }

            if (i > 0) // 清理参数
            {
                *++code = DARG;
                *++code = i;
            }

            type = tmp_ptr[Type];
        }
        else if (tmp_ptr[Class] == Enum)
        {
            *++code = IMM;
            *++code = tmp_ptr[Value]; // 枚举值
            type = INT;
        }
        else // 变量 
        {
            if (tmp_ptr[Class] == Loc)
            {
                *++code = LEA;
                *++code = ibp - tmp_ptr[Value]; // Value为ibp的偏移量
            }
            else if (tmp_ptr[Class] == Glo)
            {
                *++code = IMM;
                *++code = tmp_ptr[Value];
            }
            else
            {
                printf("line %lu: invalid variable\n", line);
                exit(-1);
            }

            type = tmp_ptr[Type];
            *++code = (type == CHAR) ? LC : LI;
        }
    }
    else if (token == '(')
    {
        assert('(');
        if (token == Char || token == Int)
        {
            tmp_type = (token == Char) ? CHAR : INT;
            tokenize();

            while (token == Mul) { assert(Mul); tmp_type += PTR; } // 解析*
            assert(')');
            type = tmp_type; // 此时强制类型转换的类型已经确定
            parse_expression(Inc);
        }
        else
        {
            parse_expression(Assign);
            assert(')');
        }
    }
    else if (token == Mul) // 解引用，用*++code来理解
    {
        assert(Mul);
        parse_expression(Inc);
        if (type >= PTR) { type -= PTR; } // type枚举设计的很精妙，PTR最大，这样可以防止int, char类型也解引用的错误
        else
        {
            printf("line %zu: invalid reference\n", line);
            exit(-1); // 语法错误，编译器必须退出，汇编代码都会生成错的
        }
        *++code = (type == CHAR) ? LC : LI; // 精妙：**a解引用是地址，也是加载LI，精简干练的代码
    }
    else if (token == And) // 取地址&，例子：&~--a
    {
        assert(And);
        parse_expression(And);
        if (*code == LC || *code == LI) { code--; }
        else
        {
            printf("line %zu: invalid reference\n", line);
            exit(-1);
        }
        type += PTR;
    }
    else if (token == '!') // 布尔值的逻辑取反
    {
        assert('!');
        parse_expression(Inc);
        *++code = PUSH;
        *++code = IMM;
        *++code = 0;
        *++code = EQ;
        type = INT; // 布尔值的类型
    }
    else if (token == '~') // 按位异或(按位取反)
    {
        assert('~');
        parse_expression(Inc);
        *++code = PUSH; // a ^ -1 = ~a, -1是全是1的二进制
        *++code = IMM;
        *++code = -1;
        *++code = XOR;
        type = INT;
    }
    else if (token == And) // +号，例如+3, +q，不是加法
    {
        assert(And);
        parse_expression(Inc);
        type = INT; // 正号什么也不用变
    }
    else if (token == Sub) // -号，例如-3, -q，不是减法
    {
        assert(And);
        parse_expression(Inc);
        *++code = PUSH; // -x = -1 * x
        *++code = IMM;
        *++code = -1;
        *++code = Mul;
        type = INT;
    }
    else if (token == Inc || token == Dec) // ++var --var
    {
        uint64_t i = token; // 存储指令，token要向下移动了
        tokenize(); // 无法使用assert，因为有两种可能
        parse_expression(Inc); // 加载变量，汇编指令：IMM address 下一条-> LI/LC

        //加载变量
        if (*code == LC)
        {
            *code = PUSH;
            *++code = LC;
        }
        else if (*code == LI)
        {
            *code = PUSH;
            *++code = LI;
        }
        else
        {
            printf("line %lu: invalid Inc or Dec(fail to load various)\n", line);
            exit(-1);
        }

        //执行自增/自减，并且存储
        *++code = PUSH;
        *++code = IMM;
        if (type > PTR) { *++code = 8; } // 例如int*, char** 
        else { *++code = 1; }
        *++code = (i == Inc) ? ADD : SUB;
        *++code = (type == CHAR) ? SC : SI; // LI包括整数和指针，一样的8字节
    }
    else
    {
        printf("line %lu: invalid expression\n", line);
        exit(-1);
    }

    // 开始优先级爬山，优先级是逐步递增的
    while (token >= precedence) // 同样的优先级也可以递归下降，我们需要不断地向右扫描，直到遇到优先级小于当前优先级的运算符。这是才处理本优先级
    {
        tmp_type = type;

        if (token == Assign)
        {
            assert(Assign); // 解析=右边的表达式：a = 表达式
            if (*code == LC || *code == LI) { *code = PUSH; }
            else
            {
                printf("line %lld: invalid assignment(fail to load various)\n", line);
                exit(-1);
            }

            parse_expression(Assign);
            type = tmp_type; // 获得类型
            *++code = (type == CHAR) ? SC : SI; // LI包括整数和指针
        }
        else if (token == Cond) // ? :
        {
            assert(Cond);
            *++code = JZ;
            tmp_ptr = (uint64_t)(++code);
            parse_expression(Assign);
            assert(':');
            *tmp_ptr = (uint64_t)(code + 3);
            *++code = JMP;
            tmp_ptr = (uint64_t)(++code);
            assert('?');
            parse_expression(Assign);
            *tmp_ptr = (uint64_t)(code + 1);
        }
        else if (token == Lor) // ||
        {
            assert(Lor);
            *++code = JNZ;
            tmp_ptr = ++code;
            parse_expression(Land);
            *tmp_ptr = (uint64_t)(code + 1);
            type = INT;
        }
        else if (token == Land) // &&
        {
            assert(Land);
            *++code = JZ;
            tmp_ptr = ++code;
            parse_expression(Or);
            *tmp_ptr = (uint64_t)(code + 1); // 要回填地址
            type = INT;
        }
        else if (token == Or) { tokenize(); *++code = PUSH; parse_expression(Xor); *++code = OR;  type = INT; }
        else if (token == Xor) { tokenize(); *++code = PUSH; parse_expression(And); *++code = Xor;  type = INT; }
        else if (token == And) { tokenize(); *++code = PUSH; parse_expression(Eq); *++code = And;  type = INT; }
        else if (token == Eq) { tokenize(); *++code = PUSH; parse_expression(Lt);  *++code = EQ;  type = INT; }
        else if (token == Ne) { tokenize(); *++code = PUSH; parse_expression(Lt);  *++code = NE;  type = INT; }
        else if (token == Lt) { tokenize(); *++code = PUSH; parse_expression(Shl); *++code = LT;  type = INT; }
        else if (token == Gt) { tokenize(); *++code = PUSH; parse_expression(Shl); *++code = GT;  type = INT; }
        else if (token == Le) { tokenize(); *++code = PUSH; parse_expression(Shl); *++code = LE;  type = INT; }
        else if (token == Ge) { tokenize(); *++code = PUSH; parse_expression(Shl); *++code = GE;  type = INT; }
        else if (token == Shl) { tokenize(); *++code = PUSH; parse_expression(Add); *++code = SHL; type = INT; }
        else if (token == Shr) { tokenize(); *++code = PUSH; parse_expression(Add); *++code = SHR; type = INT; }
        else if (token == Add) // a+b 
        {
            assert(Add);
            *++code = PUSH;
            parse_expression(Mul); // 下一级优先级

            // char* + 1 或者 int* + 3 要移动地址的字节数
            if (tmp_type > PTR)
            {
                if ((tmp_type - CHAR) % PTR == 0) // char*
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = MUL;
                }
                else if ((tmp_type - INT) % PTR == 0) // uint64_t*
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 64;
                    *++code = MUL;
                }
                else
                {
                    printf("line %lu : the pointer's type is incorrect\n", line);
                    exit(-1);
                }
            }
            *++code == ADD;
            type = tmp_type;
        }
        else if (token == Sub) // a-b 
        {
            if (tmp_type > PTR && tmp_type == type) // 同类型的p1 - p2, 指针减法时，如果是两个指针相减（相同类型），则结果是两个指针间隔的类型的个数。因此要有特殊的处理。
            {
                if ((tmp_type - CHAR) % PTR == 0) // char*, char***等类型
                {
                    *++code = SUB;
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = DIV;
                    type = INT;
                }
                else if ( (tmp_type - INT) % PTR == 0) // uint64_t*, uint64_t***等类型
                {
                    *++code = SUB;
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 64;
                    *++code = DIV;
                    type = INT;
                }
                else // 不支持其他的指针，比如int int *
                {
                    printf("line %lu : pointers' type is incorrect!\n", line);
                    exit(-1);
                }
            }
            else if (tmp_type > PTR) // (char*)a - 3; (int*)a - 3
            {
                if ((tmp_type - CHAR) % PTR == 0) // (char*)a - 3;
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = MUL;
                    *++code = SUB;
                    type = tmp_type;
                }
                else if ((tmp_type - INT) % PTR == 0) // (int*)a - 3
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 64; // 8字节
                    *++code = MUL;
                    *++code = SUB;
                    type = tmp_type;
                }
                else
                {
                    printf("line %lu : the pointer's type is incorrect\n", line);
                    exit(-1);
                }
            }
            else { *++code = SUB; } // 普通数字减法
        }
        else if (token == Mul) { tokenize(); *++code = PUSH; parse_expression(Inc); *++code = MUL; type = INT; }
        else if (token == Div) { tokenize(); *++code = PUSH; parse_expression(Inc); *++code = DIV; type = INT; }
        else if (token == Mod) { tokenize(); *++code = PUSH; parse_expression(Inc); *++code = MOD; type = INT; }
        else if (token == Inc || token == Dec) // 例如(int*)a++;
        {
            // 加载变量
            if (*code == LC)
            {
                *code = PUSH;
                *++code = LC;
            }
            else if (*code == LI)
            {
                *code = PUSH;
                *++code = LI;
            }
            else
            {
                printf("%lu: invlid operator=%lu(fail to load various)\n", line, token);
                exit(-1);
            }

            // 进行++/--，并且存储
            *++code = PUSH;
            *++code = IMM;

            // 区分int* ++, char* ++, a++
            if ((tmp_type - INT) % PTR == 0) // int*
            {
                *++code = PUSH;
                *++code = IMM;
                *++code = 64; // 8字节
                *++code = (token == Inc) ? ADD : SUB;
                *++code = SI;

                // 也要存储++/--之前的变量值
                *++code = PUSH;
                *++code == IMM;
                *++code = 64;
                *++code = (token == Inc) ? SUB : ADD;
            }
            else if ((tmp_type - CHAR) % PTR == 0) // char指针
            {
                *++code = PUSH;
                *++code = IMM;
                *++code = 8; // 1字节
                *++code = (token == Inc) ? ADD : SUB;
                *++code = SI;

                // 也要存储++/--之前的变量值
                *++code = PUSH;
                *++code == IMM;
                *++code = 8;
                *++code = (token == Inc) ? SUB : ADD;
            }
            else // 普通的类型(char, int)的运算
            {
                *++code = PUSH;
                *++code = (token == Inc) ? ADD : SUB;
                *++code = (tmp_type == CHAR) ? SC : SI;

                // 也要存储++/--之前的变量值
                *++code = PUSH;
                *++code = (token == Inc) ? SUB : ADD;
            }

            tokenize();
        }
        else if (token == Brak) // a[k] = *(a + k) 
        {
            // 分析[]内的表达式(一定会返回值)
            assert(Brak);
            *++code = PUSH;
            parse_expression(Assign);
            assert(']');

            //得到a + k
            if (tmp_type <= PTR) // 数组名一定是指针(>PTR)
            {
                printf("line %lld: invalid index op\n", line);
                exit(-1);
            }

            *++code = PUSH;
            *++code = IMM;
            *++code = (((tmp_type - INT) % PTR) == 0) ? 64 : 8;
            *++code = ADD; // 得到 a+k
            *++code = MUL; // 乘以偏移的字节数

            // 解引用得到数组的值
            type = tmp_type - PTR; // tmp_type = char*/int*
            *++code = (type == CHAR) ? LC : LI;
        }
    }
}

void parse_statement()
{
    uint64_t* tmp_1;
    uint64_t *tmp_2;

    if (token == If)
    {
        assert(If); assert('('); // 解析条件
        parse_expression(Assign);
        assert(')');

        *++code = JZ;
        tmp_1 = ++code; // 要回填false条件的起始地址

        parse_statement(); // 解析true条件下的语句

        if (token == Else)
        {
            assert(Else);
            *tmp_1 = (uint64_t)(code + 3);
            *++code = JMP;
            tmp_1 = ++code;

            parse_statement(); // 解析所以false条件的语句
        }

        *tmp_1 = (uint64_t)(code + 1); // 回填end point
    }
    else if (token == While)
    {
        assert(While);
        tmp_1 = (uint64_t)(code + 1); // 设置while的起始地址：loop point
        assert('(');
        parse_expression(Assign); // 生成ax = a > b的指令
        assert(')');

        *++code = JZ;
        tmp_2 = (uint64_t)(++code); // 设置while的终止地址：loop point
        parse_statement();

        *++code = JMP;
        *++code = (uint64_t)tmp_1;

        *tmp_2 = (uint64_t)(code + 1); // 回填end point
    }
    else if (token == Return)
    {
        assert(Return);
        if (token != ';') { parse_expression(Assign); } // 支持return x = 3;
        assert(';');
        *++code = RET; // void函数之前在parse_function处理了，这只处理显示的
    }
    else if (token == '{') // 对付循环中的语句
    {
        while (token != '}')
        {
            parse_statement();
        }
        assert('}');
    }
    else if (token = ';') { assert(';'); }
    else // 若不为statement，就是expression
    {
        parse_expression(Assign);
        assert(';'); // 看看解析完此句没有
    }
}

void parse_function()
{
    uint64_t type, i;
    i = ibp;

    while (token == Int || token == Char) // 函数存在定义的变量
    {
        type = parse_base_type();
        while (token != ';') // 解析函数的局部变量
        {
            while (token == '*')
            {
                assert(Mul);
                type += PTR;
            }
            check_local_id();

            symbol_ptr[Class] = Loc;
            symbol_ptr[Type] = type;
            symbol_ptr[Value] = ++i; // 为了得到变量的个数

            hide_and_recover_global(1);
            assert(Id);
            if (token == ',') assert(',');
        }
    }

    *++code = NVAR;
    *++code = i - ibp; // 函数变量的个数

    while (token != '}') parse_statement(); // 处理函数内容
    if (*code != RET) *++code = RET; // 为void函数补上return

    symbol_ptr = symbol_table; // 恢复全局变量+清除函数的局部变量
    while (symbol_ptr[Token])
    {
        if (symbol_ptr[Class] == Loc)
        {
            hide_and_recover_global(0); // 恢复隐匿
        }
        symbol_ptr += SymSize;
    }
}

void parser()  // 解析类型声明,包括枚举,变量和函数的定义
{
    uint64_t base_type, type;

    while (token > 0)
    {
        tokenize(); // start or skip last ; | }
        if (token == Enum)
        {
            assert(Enum);
            if (token != '{') assert(Id);
            assert('{');
            parse_enum();
            assert('}');

            return;
        }
        else if (token == Int || token == Char)
        {
            base_type = parse_base_type();

            while (token != ';' || token != '}') // 处理函数，变量
            {
                type = base_type;
                while (token == Mul)
                {
                    assert(Mul);
                    type += PTR;
                }

                // check_new_id();

                assert(Id);
                symbol_ptr[Type] = type;

                if (token == '(') // 函数
                {
                    symbol_ptr[Class] = Fun;
                    symbol_ptr[Value] = (uint64_t)(code + 1); // 函数入口地址 call adress
                    assert('(');
                    parse_param();
                    assert(')');
                    assert('{');

                    parse_function();
                }
                else // 全局变量
                {
                    symbol_ptr[Class] = Glo;
                    symbol_ptr[Value] = (uint64_t)data; // 存储值的起始地址(一共四字节)
                    data += sizeof(uint64_t);
                }

                // 处理int *a, b; int *a, *b;情况
                if (token == ',') assert(','); // 基础类型不变
            }
        }
    }
}

void keyword()  // 初始化关键字和系统调用符号表
{
    int i;
    src = "char int enum if else return sizeof while "
        "open read close printf malloc free memset memcmp exit void main";

    // 处理char~while
    i = Char;
    while (i < While)
    {
        tokenize();
        symbol_ptr[Token] = i++;
    }

    // 处理系统调用
    i = OPEN;
    while (i < EXIT)
    {
        tokenize();
        symbol_ptr[Class] = Sys;
        symbol_ptr[Type] = INT;
        symbol_ptr[Value] = i++;
    }

    // 处理void main
    tokenize(); // 得到void
    symbol_ptr[Token] = Char; // void当成char，因为没有void关键字
    tokenize();
    main_ptr = symbol_ptr;
    main_ptr[Value] = (uint64_t) stack - 9;
    printf("keyword: main ptr[Value] = %lu\n", main_ptr[Value]); // 手段得到main初始地址

    // src恢复指向源代码
    src = src_dump;
}

uint64_t initial_vm() // 0正常 -1异常
{
    // 分配汇编代码大小
    code = malloc(MAX_SIZE * 8); // 给uint64_t*分配以8字节为单位
    code_dump = code;
    code_dump_1 = code;

    if (!code)
    {
        printf("could not malloc(%lu) for code segment\n", MAX_SIZE);
        exit(-1);
    }

    // 分配data大小
    if (!(data = malloc(MAX_SIZE)))
    {
        printf("could not malloc(%lu) for data segment\n", MAX_SIZE);
    }

    // 分配stack大小
    if (!(stack = malloc(MAX_SIZE * 8)))
    {
        printf("could not malloc(%lu) for stack segment\n", MAX_SIZE);
    }

    // 分配符号表大小
    if (!(symbol_table = malloc(MAX_SIZE / 2))) {
        printf("could not malloc(%lu) for stack segment\n", MAX_SIZE / 32);
        return -1;
    }

    // 区域清零
    memset(code, 0, MAX_SIZE);
    memset(stack, 0, MAX_SIZE);
    memset(data, 0, MAX_SIZE);
    memset(symbol_table, 0, MAX_SIZE / 32);

    return 0; // 分配成功
}

uint64_t run_vm(uint64_t argc, char** argv) // exception will return -1
{
    uint64_t op; // 用于存储指令
    uint64_t* tmp; // 辅助

    // exit code for main
    bp = (uint64_t*)((uint64_t)stack + MAX_SIZE);
    sp = bp;
    *--sp = EXIT;
    *--sp = PUSH;
    tmp = sp;
    *--sp = argc;
    *--sp = (uint64_t)argv;
    *--sp = (uint64_t)tmp;

    if (!(pc = (uint64_t)main_ptr[Value])) // pc从main起始地址开始执行
    {
        printf("main function is not defined\n");
        exit(-1);
    }

    cycle = 0;
    while (0)
    {
        cycle++; op = *pc++; // 得到操作码（指令）

        // debug???????????
        // 检查指令到底写入了没有，怎么
        printf("instruction: %lld, cycle: %lld\n", *(pc-1), cycle); 
        printf("instruction: %lld, cycle: %lld\n", *pc, cycle); 
        printf("instruction: %lld, cycle: %lld\n", *(pc+1), cycle);

        // load & save
        if (op == IMM) { ax = *pc++; }
        else if (op == LEA) { ax = (int)(bp + *pc++); }
        else if (op == LC) { ax = *(char*)ax; }
        else if (op == LI) { ax = *(int*)ax; }
        else if (op == SC) { *(char*)*sp++ = ax; }
        else if (op == SI) { *(int*)*sp++ = ax; }
        else if (op == PUSH) { *--sp = ax; }

        // jump
        else if (op == JMP) { pc = (int*)*pc; }
        else if (op == JZ) { pc = (ax == 0 ? (int*)*pc : pc + 1); }
        else if (op == JNZ) { pc = (ax != 0 ? (int*)*pc : pc + 1); }

        // arithmetic
        else if (op == ADD) { ax = *sp++ + ax; }
        else if (op == SUB) { ax = *sp++ - ax; }
        else if (op == MUL) { ax = *sp++ * ax; }
        else if (op == DIV) { ax = *sp++ / ax; }
        else if (op == MOD) { ax = *sp++ % ax; }
        else if (op == OR) { ax = *sp++ | ax; }
        else if (op == XOR) { ax = *sp++ ^ ax; }
        else if (op == AND) { ax = *sp++ & ax; }
        else if (op == SHL) { ax = *sp++ << ax; }
        else if (op == SHR) { ax = *sp++ >> ax; }
        else if (op == EQ) { ax = *sp++ == ax; }
        else if (op == NE) { ax = *sp++ != ax; }
        else if (op == LT) { ax = *sp++ < ax; }
        else if (op == LE) { ax = *sp++ <= ax; }
        else if (op == GT) { ax = *sp++ > ax; }
        else if (op == GE) { ax = *sp++ >= ax; }

        // function call
        else if (op == CALL) { *--sp = (int)(pc + 1); pc = (int*)*pc; }
        else if (op == NVAR) { *--sp = (int*)bp; bp = sp; sp = sp - *pc++; }
        else if (op == RET) { sp = bp; bp = (int*)*sp++; pc = (int*)*sp++; }
        else if (op == DARG) sp = sp + *pc++;

        // native call
        else if (op == OPEN) { // MSVC支持的文件打开函数
            uint64_t fd;
            errno_t err = _sopen_s // 返回0为成功，其他值是失败
            (
                &fd,                // 返回的文件描述符指针
                (char*)sp[1],       // 文件名
                sp[0],              // 打开模式
                _SH_DENYNO,         // 共享模式（允许其他进程访问）
                _S_IREAD | _S_IWRITE // 权限模式（0644）
            );
            ax = (err == 0) ? fd : -1;  // 成功返回fd，失败返回-1
        }
        else if (op == CLOS) { ax = _close(*sp); }
        else if (op == READ) { ax = _read(sp[2], (char*)sp[1], *sp); }
        else if (op == PRTF) { tmp = sp + pc[1] - 1; ax = printf((char*)tmp[0], tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5]); }
        else if (op == MALC) { ax = (int)malloc(*sp); }
        else if (op == FREE) { free((void*)*sp); }
        else if (op == MSET) { ax = (int)memset((char*)sp[2], sp[1], *sp); }
        else if (op == MCMP) { ax = memcmp((char*)sp[2], (char*)sp[1], *sp); }
        else if (op == EXIT) { printf("exit(%lld)\n", *sp); return *sp; }
        else { printf("unkown instruction: %lld, cycle: %lld\n", op, cycle); return -1; }
    }
    return 0;
}

void write_to_file()  // 将编译过程中生成的中间代码（或汇编代码）写入到名为"assemble"的文件中
{
    uint64_t fd; // 存储文件描述符
    char* buffer = malloc(100);
    char* instruction = "IMM ,LEA ,JMP ,JZ  ,JNZ ,CALL,NVAR,DARG,RET ,LI  ,LC  ,SI  ,"
        "SC  ,PUSH,OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,"
        "SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
        "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,";

    // MSVC支持的打开文件函数_sopen_s()
    errno_t err = _sopen_s
    (
        &fd,                       // 文件描述符指针（必须传地址）
        "assemble_file",           // 文件名
        _O_WRONLY | _O_CREAT | _O_BINARY,  // 打开模式
        _SH_DENYNO,                // 共享模式（允许其他进程同时访问）
        _S_IREAD | _S_IWRITE       // 权限模式（0644）
    );

    if (err != 0) {
        printf("%lu : fd is invalided!\n", line);
        exit(-1);
    }

    printf("写入文件时：code_dump = %lu\n", code_dump);
    printf("写入文件时：code = %lu\n", code);

    // fd = open("assemble_file", _O_WRONLY | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); linux版本
    while (code_dump < code) //  每次写入一条指令
    {
        // 写入内存地址+操作码
        sprintf_s(buffer, "(%lu) %8.4s", ++code_dump, instruction + (*code_dump * 5));
        _write(fd, buffer, strlen(buffer));

        // 处理操作数的分支
        if (*code_dump <= RET) { sprintf_s(buffer, "%lu\n", *++code_dump); } // 有操作数
        else // 没有操作数
        {
            buffer[0] = '\n';
            buffer[1] = 0;
        }
        _write(fd, buffer, strlen(buffer));
    }

    _close(fd);
}

uint64_t load_src(char* code_file) // 0:正常  -1:异常
{
    uint64_t fd, count; // 文件描述符, 记录复制的字符个数

    // 打开文件，获取fd(linux version)
    //if ((fd = _sopen_s(code_file, 0)) < 0) // 只读文件模式，我们只需复制东西
    //{
    //    printf("could not open source code(%s)\n", code_file);
    //    return -1;
    //}

    // 打开文件，获取fd(MSVC version)
    errno_t err = _sopen_s
    (
        &fd,                // 返回的文件描述符指针
        code_file,          // 文件名
        _O_RDONLY | _O_BINARY,  // 只读模式 + 二进制模式
        _SH_DENYNO,         // 共享模式（允许其他进程访问）
        _S_IREAD            // 只读权限
    );

    if (err != 0 || fd < 0) {
        printf("could not open source code(%s)\n", code_file);
        return -1;
    }

    // 为src分配空间
    src = malloc(MAX_SIZE);
    src_dump = src; // 副本
    if (!src) // 若src为NULL，未分配成功
    {
        printf("could not malloc(%lu) for source code\n", MAX_SIZE);
        return -1;
    }

    // load information: file-->src
    if ((count = _read(fd, src, MAX_SIZE - 1)) <= 0) // 一次性读取; 为\0留一个位置
    {
        printf("could not read source code, MAX_SIZE = %lu\n", MAX_SIZE);
        return -1;
    }

    // +`\0`
    src[count] = 0; // 后面的都不是有用信息，截断

    // 关闭文件
    _close(fd);

    return 0;
}

// 自举后，改为uint64_t
// C/C++标准是int main, int argc
int main(int argc, char** argv)
{
    MAX_SIZE = 128 * 1024 * 64; // 1MB

    // 调试输出
    printf("argc = %llu\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (load_src(argv[1]) != 0)
    {
        printf("fail to load source !\n");
        exit(-1);
    }

    printf("\n%s\n", src);

    if (initial_vm() != 0)
    {
        printf("fail to initial vm !\n");
        exit(-1);
    }

    keyword();

    parser();

    write_to_file();

    return run_vm(--argc, ++argv); // 正常返回0，不正常返回-1
}
