#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <ctype.h>
#include <cmath>

char Priority(char& simvol) //определение приорите для операций для обратной польской записи
{
    switch (simvol)
    {
    case '+':
    case '-':
    {
        return 1;
    }
    case '*':
    case '/':
    {
        return 2;
    }
    case '^':
    {
        return 3;
    }
    }
}

void Function_add(std::string& exit, std::stack <char>& signs, std::string& s_function) // функция, которая для строк со считанными буквами, определяет дальнейшие действия
{
    if (s_function == "pi")
    {
        exit += std::to_string(3.141593);
        char probel = 32; // пробел - разграничитель, чтобы не сливались числа (в дальнейшем всё аналогично)
        exit += probel;
    }
    else if (s_function == "e")
    {
        exit += std::to_string(2.718282);
        char probel = 32;
        exit += probel;
    }
    else if (s_function == "x")
    {
        std::cout << "Enter the x argument: ";
        double x;
        std::cin >> x;
        exit += std::to_string(x);
        char probel = 32;
        exit += probel;
    }
    else if (s_function == "sin") // со всей тригонометрией и корнем идёт элементарная замена их на определённый символ для более лёгкого взаимодействия с ними
    {
        signs.push('s');
    }
    else if (s_function == "cos")
    {
        signs.push('c');
    }
    else if (s_function == "tg")
    {
        signs.push('T');
    }
    else if (s_function == "ctg")
    {
        signs.push('C'); // отличается от косинуса регистром (по ASCII совершенно разные символы, и смысл понятен)
    }
    else if (s_function == "sqrt")
    {
        signs.push('S'); //аналогично
    }
    else if (s_function == "exp") // экспоненту расписал как e^
    {
        exit += std::to_string(2.718282);
        signs.push('^');
        char probel = 32;
        exit += probel;
    }
    s_function.clear();
}

void Exit(std::string& exit, std::string& number, std::stack <char>& signs, std::string& s_function, char simvol)
{
    if (isdigit(simvol) != 0) // если цифра, то записывается в number - строка double числа (туда идёт и точка) - необходима для комплеткации числа в едино
    {
        number += simvol;
    }
    else
    {
        if (isalpha(simvol) != 0) // если буква - запись в строку функции
        {
            s_function += simvol;
        }
        else
        {
            if (simvol == '.') //точка
            {
                number += simvol;
            }
            else
            {
                if (number.length() != 0) // комплектация double числа, если number не пуст
                {
                    exit += number;
                    number.clear();
                    char probel = 32;
                    exit += probel;
                }
                if (s_function.length() != 0) // комплектация функции, если строка функция не пуста
                {
                    Function_add(exit, signs, s_function);
                }
                if (signs.size() == 0) // если стек знаков пуст - добавление знака
                {
                    signs.push(simvol);
                }
                else
                {
                    if (simvol == '(') // добавление открывающейся скобки в стек знаков (необходима для дальнейших операций)
                    {
                        signs.push(simvol);
                    }
                    else
                    {
                        if (simvol == ')') // если закрывающиеся скобка - идёт извелечение из стека знаков в exit до открывающейся скобки
                        {
                            while (signs.top() != '(')
                            {
                                exit += signs.top();
                                signs.pop();
                                char probel = 32;
                                exit += probel;
                            }
                            signs.pop(); // удаление из стека знаков открывающейся скобки
                            if (signs.size() != 0) // проверка на тригонометрию или корень и извлечение (если есть) в exit
                            {
                                if (isalpha(signs.top()) != 0)
                                {
                                    exit += signs.top();
                                    signs.pop();
                                    char probel = 32;
                                    exit += probel;
                                }
                            }
                        }
                        else
                        {
                            if (signs.top() == '(') // если в стеке последний элемент - открывающаяся скобка - добавление знака без проверки приоритета
                            {
                                signs.push(simvol);
                            }
                            else // добавление знака в стек знаков по приоритету
                            {
                                if (Priority(simvol) > Priority(signs.top()))
                                {
                                    signs.push(simvol);
                                }
                                else
                                {

                                    while ((signs.size() != 0) && (Priority(simvol) <= Priority(signs.top())))
                                    {
                                        exit += signs.top();
                                        signs.pop();
                                        char probel = 32;
                                        exit += probel;
                                    }
                                    signs.push(simvol);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Function_calculate (std::stack <double>& double_numbers, std::string& s_function) // функция вчисления
{
    if (s_function == "s") // синус
    {
        double a;
        a = double_numbers.top();
        double_numbers.pop();
        a = sin(a);
        double_numbers.push(a); // обратно в стек чисел для дальнейших вычислений
    }
    else if (s_function == "c") // косинус
    {
        double a;
        a = double_numbers.top();
        double_numbers.pop();
        a = cos(a);
        double_numbers.push(a);
    }
    else if (s_function == "T") // тангенс
    {
        double a;
        a = double_numbers.top();
        double_numbers.pop();
        a = tan(a);
        double_numbers.push(a);
    }
    else if (s_function == "C") // котангенс
    {
        double a;
        a = double_numbers.top();
        double_numbers.pop();
        a = 1 / tan(a);
        double_numbers.push(a);
    }
    else if (s_function == "S") // корень
    {
        double a;
        a = double_numbers.top();
        double_numbers.pop();
        a = sqrt(a);
        double_numbers.push(a);
    }
    else if (s_function == "+")
    {
        double a;
        double b;
        b = double_numbers.top();
        double_numbers.pop();
        a = double_numbers.top();
        double_numbers.pop();
        a += b;
        double_numbers.push(a);
    }
    else if (s_function == "-")
    {
        double a;
        double b;
        b = double_numbers.top();
        double_numbers.pop();
        a = double_numbers.top();
        double_numbers.pop();
        a -= b;
        double_numbers.push(a);
    }
    else if (s_function == "*")
    {
        double a;
        double b;
        b = double_numbers.top();
        double_numbers.pop();
        a = double_numbers.top();
        double_numbers.pop();
        a = a * b;
        double_numbers.push(a);
    }
    else if (s_function == "/")
    {
        double a;
        double b;
        b = double_numbers.top();
        double_numbers.pop();
        a = double_numbers.top();
        double_numbers.pop();
        a = a / b;
        double_numbers.push(a);
    }
    else if (s_function == "^")
    {
        double a;
        double b;
        b = double_numbers.top();
        double_numbers.pop();
        a = double_numbers.top();
        double_numbers.pop();
        a = pow(a, b);
        double_numbers.push(a);
    }
}

void Calculate (std::stack <double>& double_numbers, std::string& number, std::string& s_function, char simvol) // функция считывания exit для вчисления
{
    if (isdigit(simvol) != 0) // если цифра - запись в number для дальнейшего преобразования в double число
    {
        number += simvol;
    }
    else
    {
        if (simvol == '.') // точка - тоже для double числа
        {
            number += simvol;
        }
        else
        {
            if (number.length() != 0) // комплектация double числа
            {
                double stack_number = std::stod(number);
                double_numbers.push(stack_number);
                number.clear();
            }
            if (simvol != ' ') // если символ не пробел (пробел - сивол разделитель - нам не нужен для вычисления) - идёт вычисление
            {
                s_function += simvol;
                Function_calculate(double_numbers, s_function);
                s_function.clear();
            }
        }
    }
}

int main()
{
    std::string exit; // строка выхода для обратной польской записи
    std::string number; // строка для комплектации double чисел
    std::stack <char> signs; // стек знаков
    std::string s_function; // строка функций (считанные буквы)
    std::string expression; // само введённое пользователем выражение
    std::cout << "Enter your expression: ";
    std::getline(std::cin, expression);
    int expression_size = expression.length();
    for (int h = 0; h < expression_size; h++)
    {
        char simvol;
        simvol = expression[h]; // считывание символов
        Exit(exit, number, signs, s_function, simvol); // обратная польская запись
        if (h == (expression_size - 1)) // комплектация всего, что осталось, так как в последний считанный символ не до конца было всё собрано 
        {
            if (number.length() != 0) // косплектация последнего double числа
            {
                exit += number;
                number.clear();
                char probel = 32;
                exit += probel;
            }
            if (s_function.length() != 0) // косплектация последней функции
            {
                Function_add(exit, signs, s_function);
            }
            while (signs.size() != 0) // запись из стека знаков оставшихся знаков
            {
                exit += signs.top();
                signs.pop();
                char probel = 32;
                exit += probel;
            }
        }
    } // конец обратной польской записи - начало вычислений
    std::stack <double> double_numbers; // стек double чисел
    int exit_size = exit.length();
    for (int h = 0; h < exit_size; h++)
    {
        char simvol;
        simvol = exit[h]; // считывание символов из exit
        Calculate(double_numbers, number, s_function, simvol); // вычисление
    }
    std::cout << "\n";
    std::cout << "The result is " << double_numbers.top(); // ответ
    std::cout << "\n";
}
