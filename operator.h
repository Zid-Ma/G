#pragma once

//�ض����������ʵ��string��intƴ��
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