#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <stack>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Expected 2 arguments, got " << argc - 1 << std::endl;
        return ERROR_PARAMETER_INVALID;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        std::cerr << "Error opening input file.\n";
        return ERROR_CANNOT_OPEN_FILE;
    }

    std::stack< LN > stack;
    std::string token;
    LN firstOp, secondOp;
    bool compareResult;
    while (std::getline(inputFile, token))
    {
        try
        {
            if (token == "+")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                stack.push(firstOp + secondOp);
            }
            else if (token == "-")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                stack.push(firstOp - secondOp);
            }
            else if (token == "*")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                stack.push(firstOp * secondOp);
            }
            else if (token == "/")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                stack.push(firstOp / secondOp);
            }
            else if (token == "%")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                stack.push(firstOp % secondOp);
            }
            else if (token == "~")
            {
                firstOp = stack.top();
                stack.pop();
                stack.push(~firstOp);
            }
            else if (token == "_")
            {
                firstOp = stack.top();
                stack.pop();
                stack.push(-firstOp);
            }
            else if (token == ">=")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp >= secondOp;
                stack.emplace(LN(compareResult));
            }
            else if (token == ">")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp > secondOp;
                stack.emplace(LN(compareResult));
            }
            else if (token == "==")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp == secondOp;
                stack.emplace(LN(compareResult));
            }
            else if (token == "<=")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp <= secondOp;
                stack.emplace(LN(compareResult));
            }
            else if (token == "<")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp < secondOp;
                stack.emplace(LN(compareResult));
            }
            else if (token == "!=")
            {
                firstOp = stack.top();
                stack.pop();
                secondOp = stack.top();
                stack.pop();
                compareResult = firstOp != secondOp;
                stack.emplace(LN(compareResult));
            }
            else
            {
                std::string_view temp = std::string_view(token);
                LN ln = LN(temp);
                stack.push(ln);
            }

        } catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return ERROR_UNKNOWN;
        }
    }

    std::ofstream outputFile(argv[2]);
    if (!outputFile)
    {
        std::cerr << "Error opening output file.\n";
        return ERROR_CANNOT_OPEN_FILE;
    }

    while (!stack.empty())
    {
        LN number = stack.top();
        number.hexaDecimalPrint(outputFile);
        stack.pop();
    }

    return SUCCESS;
}