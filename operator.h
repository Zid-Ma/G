#pragma once

//重定义操作符，实现string与int拼接
string operator+(string& content, int number)
{
    string temp = "";
    char t = 0;
    while (true)
    {
        t = number % 10 + '0';
        temp = t + temp;
        number /= 10;
        if (number == 0)
        {
            return content + temp;
        }
    }
}

string& operator+=(string& content, int number)
{
    return content = content + number;
}