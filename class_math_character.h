#pragma once
static bitset<8> CharToByte(char _s)
{
    char s = _s;
    //字符串转化成二进制串
    bitset<8> bits = bitset<8>(s);
    bitset<1> tmp;
    //二进制翻转
    //for (int i = 0; i < 4; i++)
    //{
    //    tmp[0] = bits[i];
    //    bits[i] = bits[7 - i];
    //    bits[7 - i] = tmp[0];
    //}
    return bits;
}

static bitset<16> WCharToByte(wchar_t _s)
{
    //字符串转化成二进制串
    bitset<16> bits = bitset<16>(_s);
    bitset<1> tmp;
    //二进制翻转
    //for (int i = 0; i < 4; i++)
    //{
    //    tmp[0] = bits[i];
    //    bits[i] = bits[7 - i];
    //    bits[7 - i] = tmp[0];
    //}
    return bits;
}

std::wstring ANSIToUnicode(const std::string& str);
static bitset<8> StringToByte(string _s)
{
    char s = _s.c_str()[0];
    //字符串转化成二进制串
    bitset<8> bits = bitset<8>(s);
    bitset<1> tmp;
    //二进制翻转
    for (int i = 0; i < 4; i++)
    {
        tmp[0] = bits[i];
        bits[i] = bits[7 - i];
        bits[7 - i] = tmp[0];
    }
    return bits;
}

static bitset<16> StringToByte16(string _s)
{
    //字符串转化成二进制串
    bitset<16> bits;
    char s[2] = { _s[0], _s[1] };
    bitset<8> b8;
    b8 = CharToByte(s[0]);
    for (int i = 0; i < 8; i++)
    {
        bits[i] = b8[i];
    }
    b8 = CharToByte(s[1]);
    for (int i = 0; i < 8; i++)
    {
        bits[i + 8] = b8[i];
    }
    return bits;
}

static bitset<16> WStringToByte16(wstring _s)
{
    //字符串转化成二进制串
    bitset<16> bits;
    const wchar_t* s = _s.c_str();
    //char cs = (char)s[0];
    //cout << *s << endl;
    bits = WCharToByte(s[0]);
    return bits;
}

//将字符转为全角
static std::string ToFull(std::string str)
{
    std::string result = "";
    unsigned char tmp; unsigned char tmp1;
    for (unsigned int i = 0; i < str.length(); i++)
    {
        tmp = str[i];
        tmp1 = str[i + 1];
        //cout << "uchar:" << (int) tmp << endl;
        if (tmp > 32 && tmp < 128)
        {//是半角字符
            result += 163;//第一个字节设置为163
            result += (unsigned char)str[i] + 128;//第二个字节+128;
        }
        else if (tmp >= 163)
        {//是全角字符  
            result += str.substr(i, 2);
            i++;
            continue;
        }
        else if (tmp == 32)
        {//处理半角空格  
            result += 161;
            result += 161;
        }
        else
        {
            result += str.substr(i, 2);
            i++;
        }
    }
    return result;
}

//将字符转为半角
static string ToHalf(string str) {
    string result = "";
    unsigned char tmp; unsigned char tmp1;
    for (unsigned int i = 0; i < str.length(); i++) {
        tmp = str[i];
        tmp1 = str[i + 1];
        cout << "uchar:" << (int)tmp << endl;
        if (tmp == 163) {///第一个字节是163，标志着是全角字符   
            result += (unsigned char)str[i + 1] - 128;
            i++;
            continue;
        }
        else if (tmp > 163) {//汉字   
            result += str.substr(i, 2);
            i++;
            continue;
        }
        else if (tmp == 161 && tmp1 == 161) {///处理全角空格   
            result += "";
            i++;
        }
        else {
            result += str.substr(i, 1);
        }
    } return result;
}

//GB32转化为Unicode
static void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
    return;
}
//UTF8转化为Unicode
static void UTF_8ToUnicode(WCHAR* pOut, char* pText)
{
    char* uchar = (char*)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}
//Unicode转UTF_8
static void  UnicodeToUTF_8(char* pOut, WCHAR* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char*)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));

    return;
}
//GB32转化为UTF8
static string GB2312ToUTF_8(char* pText, int pLen)
{
    string pOut;
    char buf[4];
    memset(buf, 0, 4);

    pOut.clear();

    int i = 0;
    while (i < pLen)
    {
        //如果是英文直接复制就可以
        if (pText[i] >= 0)
        {
            char asciistr[2] = { 0 };
            asciistr[0] = (pText[i++]);
            pOut.append(asciistr);
        }
        else
        {
            WCHAR pbuffer;
            Gb2312ToUnicode(&pbuffer, pText + i);

            UnicodeToUTF_8(buf, &pbuffer);

            pOut.append(buf);

            i += 2;
        }
    }

    return pOut;
}

//GB32(Ansi)转化为UTF8
static string GBtoUTF8(const char* strAnsi)
{
    //获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页  
    int nLen = MultiByteToWideChar(CP_ACP, NULL, strAnsi, -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(CP_ACP, NULL, strAnsi, -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;
    //获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区  
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR* szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    string s1 = szBuffer;
    //内存清理  
    delete[]wszBuffer;
    delete[]szBuffer;
    return s1;
}

//将UTF8编码格式转化为GBT32
static string UTF8ToGB(const char* str)
{
    string result;
    WCHAR* strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;
}

static std::string UnicodeToANSI(const std::wstring& wstr)
{
    std::string ret;
    std::mbstate_t state = {};
    const wchar_t* src = wstr.data();
    size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< char[] > buff(new char[len + 1]);
        len = std::wcsrtombs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

static std::wstring ANSIToUnicode(const std::string& str)
{
    std::wstring ret;
    std::mbstate_t state = {};
    const char* src = str.data();
    size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
        len = std::mbsrtowcs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

//将ANSI-string转化为Unicode-wstring
static std::wstring StringToWString(const std::string& str)
{
    int num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wide = new wchar_t[num];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wide, num);
    std::wstring w_str(wide);
    delete[] wide;
    return w_str;
}

/// <summary>
/// 将string类型转化为float类型
/// </summary>
/// <param name="s">文本内容：!负号使用下划线'_'代替</param>
/// <returns>浮点型</returns>
static float stringTofloat(string s)
{
    if (s != "")
    {
        if (s.substr(0, 1) != "_" || s.substr(0,1) == "-")
        {
            bool front = true;
            string NihaoWanjia, b;
            for (int i = 0; i < s.length(); i++)
            {
                if (s.substr(i, 1) == ".")
                {
                    front = false;
                }
                else if (front == true)
                {
                    NihaoWanjia.append(s.substr(i, 1));
                }
                else
                {
                    if (s.substr(i, 1) != "f")
                        b.append(s.substr(i, 1));
                }
            }
            if (NihaoWanjia.length() == 0)
            {
                NihaoWanjia = "0";
            }
            if (b.length() == 0)
            {
                b = "0";
            }
            int z = stoi(NihaoWanjia);
            int x = stoi(b);
            //Print::Line(to_string(0.1f * b.lengthCout()));
            return 1.0f * ((z * NihaoWanjia.length()) + (x * (0.1f * b.length())));
        }
        else
        {
            s = s.substr(1);
            bool front = true;
            string NihaoWanjia, b;
            for (int i = 0; i < s.length(); i++)
            {
                if (s.substr(i, 1) == ".")
                {
                    front = false;
                }
                else if (front == true)
                {
                    NihaoWanjia.append(s.substr(i, 1));
                }
                else
                {
                    if (s.substr(i, 1) != "f")
                        b.append(s.substr(i, 1));
                }
            }
            if (NihaoWanjia.length() == 0)
            {
                NihaoWanjia = "0";
            }
            if (b.length() == 0)
            {
                b = "0";
            }
            int z = stoi(NihaoWanjia);
            int x = stoi(b);
            //Print::Line("负数-" + NihaoWanjia + "." + b);
            return -1.0f * ((z * NihaoWanjia.length()) + (x * (0.1f * b.length())));
        }
    }
    return 0.0f;
}
/// <summary>
/// 将string类型转为vec3类型
/// </summary>
/// <param name="_s">内容</param>
/// <returns>vec3</returns>
static glm::vec3 StringToVec3(string _s)
{
    glm::vec3 vec;
    int vec_count = 0;
    string s_count = "";
    for (int i = 0; i < _s.length(); i++)
    {
        if (_s.substr(i, 1) != ",")
        {
            s_count.append(_s.substr(i, 1));
        }
        else
        {
            vec[vec_count] = stringTofloat(s_count);
            if (vec_count >= 2)
                break;
            vec_count++;
            s_count = "";
        }
    }
    if (vec_count <= 2)
        vec[vec_count] = stringTofloat(s_count);
    return vec;
}