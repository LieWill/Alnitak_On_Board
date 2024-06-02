#include "FUNTION_IMAGE.H"
#include <string.h>
#include <math.h>
#include <stdio.h>

#define E (double)2.71828
#define Pi (double)3.14159

int operaterPriority(int priority, char operater)
{
    if ((operater == 'n' || operater == 's' || operater == 't' || operater == 'g' || operater == 'e' || operater == 'a') && priority < 4)
        return 1;
    else if ((operater == '^') && priority < 4)
        return 1;
    else if ((operater == '*' || operater == '/' || operater == ' ') && priority < 3)
        return 1;
    else if ((operater == '+' || operater == '-') && priority < 2)
        return 1;
    else if (operater == ')')
        return 1;
    return 0;
}

double operate(char operator, double left_value, double right_value)
{
    switch (operator)
    {
    case '+':
        return left_value + right_value;
    case '-':
        return left_value - right_value;
    case ' ':
    case '*':
        return left_value * right_value;
    case '/':
        return left_value / right_value;
    case '^':
        return pow(left_value, right_value);
    case 'n':
        return sin(right_value);
    case 's':
        return cos(right_value);
    case 't':
        return tan(right_value);
    case 'g':
        return log10f(right_value) / log10f(left_value);
    case 'a':
        return fabs(right_value);
    case 'e':
        return log(right_value);
    case 0:
        return right_value;
    }
    return 0;
}

int DrawFuntionImage(char *funtion, double x, double y, double error)
{
    double number[20] = {0};
    char operater[30] = {0};
    int number_pointer = 1, operater_pointer = 1, priority = 0;
    double right = 0, left = y;
    char operation_mode = 0;
    char mode = '=';
    for (int i = 0; i < strlen(funtion); i++)
    {
        int dot = 0;
        switch (funtion[i])
        {
        case 'y':
        case 'Y':
            number[number_pointer++] = y;
            break;
        case 'X':
        case 'x':
            number[number_pointer++] = x;
            break;
        case 'e':
        case 226: // 226就是?
            number[number_pointer++] = E;
            break;
        case 'p':
        case 207: // 207就是π
            number[number_pointer++] = Pi;
        case ',':
            break;
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            while ((funtion[i] > 47 && funtion[i] < 58) || funtion[i] == '.')
            {
                if (funtion[i] != '.')
                {
                    number[number_pointer] *= 10;
                    number[number_pointer] += funtion[i++] - 48;
                    if (dot)
                    {
                        dot++;
                    }
                }
                else
                {
                    dot++;
                    i++;
                }
            }
            if (dot)
            {
                while (--dot)
                {
                    number[number_pointer] /= 10;
                }
            }
            i--;
            number_pointer++;
            operation_mode = 1;
            break;
        case 'l':
            if (funtion[i + 1] == 'n')
            {
                i++;
                funtion[i] = 'e';
            }
        case 's':
        case 'c':
        case 't':
        case 'a':
            if ((funtion[i + 1] == 'i' && funtion[i + 2] == 'n') || (funtion[i + 1] == 'o' && (funtion[i + 2] == 's' || funtion[i + 2] == 'g')))
                i += 2;
            else if (funtion[i + 1] == 'a' && funtion[i + 2] == 'n')
            {
                i += 2;
                funtion[i] = 't';
            }
            else if (funtion[i + 1] == 'b' && funtion[i + 2] == 's')
            {
                i += 2;
                funtion[i] = 'a';
            }
        case '(':
            priority++;
        case '^':
            priority++;
        case '*':
        case '/':
        case ' ':
            priority++;
        case '+':
        case '-':
            priority++;
        case ')':
            if (funtion[i] == ')' && operater[operater_pointer] == '(')
            {
                operater_pointer--;
                if (operater_pointer == 0)
                    operater[++operater_pointer] = 0;
                break;
            }
            if (operater_pointer != 0 && operaterPriority(priority, operater[operater_pointer]))
            {
                number_pointer--;
                if (operater[operater_pointer] != 'n' && operater[operater_pointer] != 's' && operater[operater_pointer] != 't' && operater[operater_pointer] != 'e')
                {
                    if (operater[operater_pointer] == '-' && operater[operater_pointer - 1] == '(' && operation_mode != 1)
                        number[number_pointer++] = operate('-', 0, number[number_pointer]);
                    else
                    {
                        number[number_pointer - 1] = operate(operater[operater_pointer], number[number_pointer - 1], number[number_pointer]);
                        number[number_pointer] = 0;
                    }
                }
                else
                    number[number_pointer++] = operate(operater[operater_pointer], number[number_pointer - 1], number[number_pointer]);
            }
            else if (operater[operater_pointer] != 0)
                operater_pointer++;
            operater[operater_pointer] = funtion[i];
            operation_mode = 0;
            break;
        case '<':
        case '>':
            if (funtion[i] == '<')
                mode = '<';
            else if (funtion[i] == '>')
                mode = '>';
        case '=':
            number_pointer--;
            for (; operater_pointer > 0; operater_pointer--)
            {
                if (number_pointer > 1)
                {
                    number_pointer--;
                    if (operater[operater_pointer] != 'n' && operater[operater_pointer] != 's' && operater[operater_pointer] != 't' && operater[operater_pointer] != 'e')
                        if (operater[operater_pointer] == '-' && operater[operater_pointer - 1] == '(' && operation_mode != 1)
                            number[number_pointer] = operate('-', 0, number[number_pointer]);
                        else
                            number[number_pointer] = operate(operater[operater_pointer], number[number_pointer], number[number_pointer + 1]);
                    else
                    {
                        number[++number_pointer] = operate(operater[operater_pointer], number[number_pointer], number[number_pointer + 1]);
                    }
                }
                else
                    number[number_pointer] = operate(operater[operater_pointer], number[number_pointer - 1], number[number_pointer]);
            }
            left = number[number_pointer];
            operater[1] = 0;
            number[1] = 0;
            operater_pointer = 1;
            number_pointer = 1;
            operation_mode = 0;
            break;
        default:
            break;
        }
        if (operater[operater_pointer] == ')')
        {
            if (operater[operater_pointer - 1] == '(')
            {
                operater_pointer -= 2;
                if (operater_pointer == 0)
                    operater[++operater_pointer] = 0;
            }
            else
            {
                i--;
                operater_pointer--;
            }
        }
        priority = 0;
    }
    number_pointer--;
    for (; operater_pointer > 0; operater_pointer--)
    {
        if (number_pointer > 1)
        {
            number_pointer--;
            if (operater[operater_pointer] != 'n' && operater[operater_pointer] != 's' && operater[operater_pointer] != 't' && operater[operater_pointer] != 'e')
                if (operater[operater_pointer] == '-' && operater[operater_pointer - 1] == '(' && operation_mode != 1)
                    number[number_pointer] = operate('-', 0, number[number_pointer]);
                else
                    number[number_pointer] = operate(operater[operater_pointer], number[number_pointer], number[number_pointer + 1]);
            else
            {
                number[++number_pointer] = operate(operater[operater_pointer], number[number_pointer], number[number_pointer + 1]);
            }
        }
        else
            number[number_pointer] = operate(operater[operater_pointer], number[number_pointer - 1], number[number_pointer]);
    }
    right = number[number_pointer];
    // double err = left - right;
    //  if (mode == '=') {
    //      if ((err >= -error && err <= error))
    //          return 1;
    //      else
    //          return 0;
    //  } else if (mode == '<') {
    //      if (err < error)
    //          return 1;
    //      else
    //          return 0;
    //  } else if (mode == '>') {
    //      if (err > error)
    //          return 1;
    //      else
    //          return 0;
    //  }
    //  return 0;
    if (mode == '=')
    {
        if (right == left)
            return 0;
        if (right > left)
            return 1;
        if (right < left)
            return -1;
    }
    else if (mode == '>')
    {
        if (right == left)
            return 0;
        if (right > left)
            return -1;
        if (right < left)
            return 2;
    }
    else if (mode == '<')
    {
        if (right == left)
            return 0;
        if (right > left)
            return 2;
        if (right < left)
            return -1;
    }
    return 11;
}
