#pragma once
static UI* ui;

class Font
{
private:
    struct Text
    {
        string path;
        string content;
        int pixels;
        int image_width;
        int image_height;
        unsigned char* contents;
    };
    bool CheckText(Font::Text &t1, Font::Text &t2)
    {
        if (t1.path == t2.path && t1.content == t2.content && t1.pixels == t2.pixels && t1.image_width == t2.image_width && t1.image_height == t2.image_height)
            return true;
        else
            return false;
    }
private:
    vector<Font::Text> texts;
public:
    int CheckText(string _path, string _text, int _pixels, int _image_width, int _image_height)
    {
        Text t;
        t.path = _path;
        t.content = _text;
        t.pixels = _pixels;
        t.image_width = _image_width;
        t.image_height = _image_height;
        int iCount = 0;
        for (int i = 0; i < texts.size(); i++)
        {
            if (CheckText(t, texts[i]))
            {
                iCount = i;
                return iCount;
                break;
            }
        }
        return -1;
    }

/// <summary>
/// ��������ΪPNGͼ��
/// </summary>
/// <param name="_load_mode">����ģʽ��m_���ص��ڴ�ռ� d_���ص����̿ռ�</param>
/// <param name="_path">����·��</param>
/// <param name="_text">�ı�����</param>
/// <param name="_pixels">�����С</param>
/// <param name="_image_width">��Ҫ���ɵ�ͼƬ���</param>
/// <param name="_image_height">��Ҫ���ɵ�ͼƬ�߶�</param>
/// <param name="_out_len">���ص����ݳ���</param>
/// <returns>ͼ������</returns>
static unsigned char* Load_FontToPNG(string _load_mode, string _path, string _text, int _pixels, int _image_width, int _image_height, int* _out_len)
{
    string _debug = "���������Ϊͼ��ʱ��";
    _text = LanguageToLanguage(_text);
    /* �������壨.ttf���ļ� */
    long int size = 0;
    unsigned char* fontBuffer = NULL;

    //������
    FILE* fontFile = fopen(_path.c_str(), "rb");//rb - ���Զ�ȡ�Ķ������ļ�
    if (fontFile == NULL)
    {
        Print::Debug(_debug + "Can not open font file!\n");
        return NULL;
    }
    fseek(fontFile, 0, SEEK_END); /* �����ļ�ָ�뵽�ļ�β�������ļ�βƫ��0�ֽ� */
    size = ftell(fontFile);       /* ��ȡ�ļ���С���ļ�β - �ļ�ͷ  ��λ���ֽڣ� */
    fseek(fontFile, 0, SEEK_SET); /* ���������ļ�ָ�뵽�ļ�ͷ */
    //Print::Debug(to_string(size));
    //��̬����һ���ڴ�ռ�
    fontBuffer = (unsigned char*)calloc(size, sizeof(unsigned char));
    //fontBuffer = new unsigned char[size];
    //fontBuffer = new unsigned char(size * sizeof(unsigned char));
    fread(fontBuffer, size, 1, fontFile);//���ļ����е����ݶ�ȡ���ڴ���
    fclose(fontFile);//�ر��ļ���
    if (fontBuffer == NULL)
    {
        Print::Debug(_debug + "���建��Ϊ��");
    }

    //ifstream fin;
    ////���ö����ƴ��ļ�
    //fin.open(_path, ios::binary);
    //if (fin.is_open())
    //{
    //    ////��ȡ�ļ���С
    //    //fin.seekg(0, ios::end);
    //    //size = fin.tellg();
    //    //fin.seekg(0, ios::beg);
    //    filebuf* pbuf;
    //    pbuf = fin.rdbuf();
    //    // ����buffer���󷽷���ȡ�ļ���С
    //    size = pbuf->pubseekoff(0, ios::end, ios::beg);
    //    pbuf->pubseekpos(0, ios::in);
    //    //������Ӧ���ڴ�ռ�
    //    //fontBuffer = new unsigned char(size);
    //    char* _c = new char[size];
    //    // ��ȡ�ļ�����
    //    pbuf->sgetn(_c, size);
    //    fontBuffer = new unsigned char[size];
    //    for (int i = 0; i < size; i++)
    //    {
    //        fontBuffer[i] = (unsigned char)_c[i];
    //    }
    //    delete[] _c;
    //    fin.close();
    //}

    //count << "???0" << endl;
    /* ��ʼ������ */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        Print::Debug(_debug + "stb init font failed\n");
    }
    //count << "???1" << endl;
    /* ����λͼ */
    int bitmap_w = 256; /* λͼ�Ŀ� */
    bitmap_w = _image_width;
    int bitmap_h = 256; /* λͼ�ĸ� */
    bitmap_h = _image_height;
    unsigned char* bitmap = (unsigned char*)calloc(bitmap_w * bitmap_h, sizeof(unsigned char));
    //unsigned char* bitmap = new unsigned char[bitmap_w * bitmap_h]();
    //unsigned char* bitmap = new unsigned char(bitmap_w * bitmap_h * sizeof(unsigned char));///sizeof(unsigned char)

    /* "STB"�� unicode ���� */
    //char word[20] = { 0x53, 0x54, 0x42, 0xa123};
    string ss = "��ã����磡";//u0x554b
    ss = _text;
    wstring ws = StringToWString(ss);
    int word[50] = {};//{ 0x53, 0x54, 0x42, 0x8c01 };
    int word_size = ws.size();
    for (int i = 0; i < ws.size(); i++)
    {
        bitset<16> bi = WStringToByte16(ws.substr(i, 1));
        int  x = bi.to_ulong();
        word[i] = x;
    }
    //Print::Debug(to_string(word[4]));

    /* ������������ */
    float pixels = 32.0;                                    /* �����С���ֺţ� */
    pixels = _pixels;
    float scale = stbtt_ScaleForPixelHeight(&info, pixels); /* scale = pixels / (ascent - descent) */
    //count << "???2" << endl;
    /**
     * ��ȡ��ֱ�����ϵĶ���
     * ascent������ӻ��ߵ������ĸ߶ȣ�
     * descent�����ߵ��ײ��ĸ߶ȣ�ͨ��Ϊ��ֵ��
     * lineGap����������֮��ļ�ࣻ
     * �м��Ϊ��ascent - descent + lineGap��
    */
    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    //count << "???3" << endl;
    /* �������ŵ����ָ� */
    ascent = roundf(ascent * scale);//����
    descent = roundf(descent * scale);//�½�

    int x = 0; /*λͼ��x*/
    int y = 0, ycount = 0;
    bool first_word = true;
    /* ѭ������word��ÿ���ַ� */
    for (int i = 0; i < word_size; ++i)
    {
        /**
          * ��ȡˮƽ�����ϵĶ���
          * advanceWidth���ֿ�
          * leftSideBearing�����λ�ã�
        */
        int advanceWidth = 0;
        int leftSideBearing = 0;
        stbtt_GetCodepointHMetrics(&info, word[i], &advanceWidth, &leftSideBearing);

        //count << "???4" << endl;
        /* ��ȡ�ַ��ı߿򣨱߽磩 */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        /* ����λͼ��y (��ͬ�ַ��ĸ߶Ȳ�ͬ�� */
        y = ascent + c_y1 + ycount;
        if (first_word)
        {
            //y = ascent + c_y1;
            first_word = false;
        }

        /* ��Ⱦ�ַ� */
        int byteOffset = x + roundf(leftSideBearing * scale) + (y * bitmap_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, word[i]);

        /* ����x */
        x += roundf(advanceWidth * scale);//�������λ������

        /* �����־� */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);//�������λ������

        //count << x << "+" << y << endl;
        if (x >= 256)
        {
            x = 0;
            ycount += ascent + c_y1;
            ycount += roundf(advanceWidth * scale);
            ycount += roundf(kern * scale);//�������λ������
        }
    }
    unsigned char* png = NULL;
    int len = 1;
    if (_load_mode == "d")
    {
        /* ��λͼ���ݱ��浽1ͨ����pngͼ���� */
        stbi_write_png((Path_Font + "STB.png").c_str(), bitmap_w, bitmap_h, 1, bitmap, bitmap_w);
    }
    else
    {
        png = stbi_write_png_to_mem(bitmap, bitmap_w, bitmap_w, bitmap_h, 1, &len);
        *_out_len = len;
    }
    //����̬�������ڴ�ռ��ͷ�
    free(fontBuffer);
    //delete[] fontBuffer;
    free(bitmap);
    //delete[] bitmap;

    return png;

    //return ;
}
public:
    Font()
    {

    }

    ~Font()
    {

    }
};

struct UI_Font
{
    string name = "";//�ڴ�����е�ʶ������
    vec3 point;//����
    vec3 lwh;//�����
    vec4 color = vec4(0.0f);//RGBA
    GLuint texture;//OpenGL�е�����������
    unsigned char* content;//ͼƬ��Դ��
    int length;//ͼƬ��Դ�볤��
    void* function = NULL;//��Ҫע���������ĺ���
    void* hover = PrintHover;//�����ͣʱ�ļ����
    void* press = PrintPress;//��갴��ʱ�ļ����
    void* release = PrintRelease;//����ɿ�ʱ�ļ����
    int mod = -1;//��ǰ����ʾģʽ
    int type = 0;//0Ϊ��������,1Ϊ��ͨͼƬ
    GLuint textures[6] = { 0,0,0,0,0,0 };//ͼ����OpenGL�е���������
};
static string Font = "fangzhengheiti.TTF";//�����õ������ʽ

class MouseClickEvent
{
private:
    struct Information
    {
        //���� Square:2DԲ�Σ�2D����
        string type;
        //״̬ down/up/press/release/hover
        string state;
        //����λ��
        vec3 point;
        //����λ�õĳߴ�(�����)/(�뾶)
        vec3 lwh;
    };
    typedef void (*function)();
public:
    Information information;
    function Function;
    UI_Font* ui_Font;
public:
    MouseClickEvent()
    {

    }

    MouseClickEvent(string _type, string _state, vec3 _point, vec3 _lwh)
    {
        information.type = _type;
        information.state = _state;
        information.point = _point;
        information.lwh = _lwh;
    }

    MouseClickEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function)
    {
        information.type = _type;
        information.state = _state;
        information.point = _point;
        information.lwh = _lwh;
        Function = (function)_function;
    }

    MouseClickEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function, UI_Font* _ui_font)
    {
        information.type = _type;
        information.state = _state;
        information.point = _point;
        information.lwh = _lwh;
        ui_Font = _ui_font;
        Function = (function)_function;
    }

    void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function, UI_Font* _ui_font)
    {
        information.type = _type;
        information.state = _state;
        information.point = _point;
        information.lwh = _lwh;
        Function = (function)_function;
    }

    void Event()
    {
        Function();
    }

    bool CheckEvent(vec3 _all_point)
    {
        return CheckEvent(_all_point, information.state);
    }

    //����Ƿ񼤻��¼�(�¼����꣬ģʽ)
    bool CheckEvent(vec3 _all_point,string _state)
    {
        if (_state != information.state)
        {
            return false;
        }
        if (information.type == "Square")
        {
            if (distance(_all_point, information.point) < information.lwh.r)
            {
                return true;
            }
        }
        else
        {
            //�������������С��
            vec2 x = vec2(information.point.r + (information.lwh.r * 0.5f), information.point.r - (information.lwh.r * 0.5f));
            vec2 y = vec2(information.point.g + (information.lwh.g * 0.5f), information.point.g - (information.lwh.g * 0.5f));
            vec2 z = vec2(information.point.b + (information.lwh.b * 0.5f), information.point.b - (information.lwh.b * 0.5f));
            if (false && _state == "click")
            {
                Print::Line("clickPoint:" + to_string(_all_point.r) + "/" + to_string(_all_point.g) + "/" + to_string(_all_point.b));
                Print::Line("Point:" + to_string(information.point.r) + "/" + to_string(information.point.g) + "/" + to_string(information.point.b));
                Print::Line("lwh:" + to_string(information.lwh.r) + "/" + to_string(information.lwh.g) + "/" + to_string(information.lwh.b));
                Print::Line("x:" + to_string(x.r) + "/" + to_string(x.g));
                Print::Line("y:" + to_string(y.r) + "/" + to_string(y.g));
                Print::Line("z:" + to_string(z.r) + "/" + to_string(z.g));
            }
            if (_all_point.r <= x.r && _all_point.r >= x.g)
            {
                if (_all_point.g <= y.r && _all_point.g >= y.g)
                {
                    if (_all_point.b <= z.r && _all_point.b >= z.g)
                    {
                        //Print::Line("true");
                        return true;
                    }
                }
            }
        }
        //Print::Line("false");
        return false;
    }
};

class UI_MessageBox
{
private:
    static bool first;
private:
    static UI_Font background;
    static UI_Font confirm_button;
    static UI_Font determine_button;
    static UI_Font cancel_button;
    UI_Font font;
private:
    string mess;
    int type;
public:
    UI_MessageBox()
    {
        First();
    }
    ~UI_MessageBox()
    {
        Functions.clear();
    }
    void First()
    {
        if (first)
        {
            InitalizationThis();
            first = false;
        }
    }
    void Initialization(string _mess, int _type, void* _function0, void* _function1)
    {
        mess = _mess;
        type = _type;
        Functions.push_back(_function0);
        Functions.push_back(_function1);
        InitalizationThisFont(_mess, NULL);
        if (_function1 != NULL)
        {
            ThisRegisterEvent(_type);
        }
        else
        {
            ThisRegisterEvent(_type);
        }
    }
    void DrawMessageBox()//_type 0��ʾ��֪ͨ�� _type 1��ʾȷ�Ϻ�ȡ��
    {
        int _collection_count = 3;
        int _collection[5] = { 0,2,3,4,0 };
        if (type == 0)
        {
            _collection_count = 3;
            _collection[0] = 0;
            _collection[1] = 1;
            _collection[2] = 4;
        }
        else 
        {
            _collection_count = 4;
        }
        vector<UI_Font> fonts;
        fonts.push_back(background);
        fonts.push_back(confirm_button);
        fonts.push_back(determine_button);
        fonts.push_back(cancel_button);
        fonts.push_back(font);
        for (int i = 0; i < _collection_count; i++)
        {
            if (_collection[i] != 0)
            {
                ui->Use_fontShader();
                UI_Font* ui_font = &fonts[_collection[i]];
                ui->DrawImage(ui_font->texture, ui_font->point.x, ui_font->point.y, ui_font->lwh.x, ui_font->lwh.y);
            }
            else
            {
                ui->Use_shader();
                UI_Font* ui_font = &fonts[_collection[i]];
                ui->DrawImage(ui_font->texture, ui_font->point.x, ui_font->point.y, ui_font->lwh.x, ui_font->lwh.y);
            }
        }
    }
    void ThisRegisterEvent(int _type)
    {
        int _collection_count = 3;
        int _collection[5] = { 0,2,3,4,0 };
        //ע��ȷ����Ϣ��
        if (_type == 0)
        {
            _collection_count = 3;
            _collection[0] = 0;
            _collection[1] = 1;
            _collection[2] = 4;
        }
        else
        {
            _collection_count = 4;
        }
        vector<UI_Font> fonts;
        fonts.push_back(background);
        fonts.push_back(confirm_button);
        fonts.push_back(determine_button);
        fonts.push_back(cancel_button);
        fonts.push_back(font);
        for (int i = 0; i < _collection_count; i++)
        {
            if (fonts[_collection[i]].function != NULL)
                RegisterEvent("", "click", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].function, (void*)&fonts[i]);
        }
    }
private:
    static void InitalizationThis()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        _point = vec3(0, 0, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(350, 200, 0);
        _f.content = NULL;
        Load_texture(&_f.texture, Path_Resources + "ui/MessageBoxBackground.png");
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        background = _f;
        Print::Debug("???");
        _point = vec3(0, -90, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(100, 50, 0);
        _f.content = Font::Load_FontToPNG("m", Path_Font + Font, "ȷ��", 30, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.texture = Load_texture_image(_f.content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = Confirm;
        confirm_button = _f;
        Print::Debug("???");
        _point = vec3(-80, -90, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(100, 50, 0);
        _f.content = Font::Load_FontToPNG("m", Path_Font + Font, "ȷ��", 30, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.texture = Load_texture_image(_f.content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = Determine;
        determine_button = _f;
        Print::Debug("???");
        _point = vec3(80, -90, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(100, 50, 0);
        _f.content = Font::Load_FontToPNG("m", Path_Font + Font, "ȡ��", 30, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.texture = Load_texture_image(_f.content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = Cancel;
        cancel_button = _f;
    }
    void InitalizationThisFont(string _s, void* _function)
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        _point = vec3(0, 20, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(345, 150, 0);
        _f.content = Font::Load_FontToPNG("m", Path_Font + Font, _s, 30, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.texture = Load_texture_image(_f.content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = _function;
        font = _f;
    }
public:
    static int Push;//0 ; 1,2,3
    static vector<void*> Functions;
private:
    typedef void (*func)();
        static void Confirm()
        {
            func f = (func)Functions[0];
            f();
        }
        static void Determine()
        {
            func f = (func)Functions[0];
            f();
        }
        static void Cancel()
        {
            func f = (func)Functions[1];
            f();
        }
};
bool UI_MessageBox::first = true;
UI_Font UI_MessageBox::background;
UI_Font UI_MessageBox::confirm_button;
UI_Font UI_MessageBox::determine_button;
UI_Font UI_MessageBox::cancel_button;
int UI_MessageBox::Push = true;
vector<void*> UI_MessageBox::Functions;

static UI_MessageBox* ui_MessageBox;

UI_Font New_UI_Font(string _name, vec3 _point, vec3 _lwh, int _pixel, int _mod, void* _function, string _content, string _content0, string _content1, string _content2, string _content3, string _content4)
{
    UI_Font _f;
    unsigned char* content;
    _f.point = _point;
    _f.lwh = _lwh;
    _f.name = _name;
    if (_content0 != "")
    {
        content = Font::Load_FontToPNG("m", Path_Font + Font, _content0, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.textures[0] = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        delete[] content;
    }
    if (_content1 != "")
    {
        content = Font::Load_FontToPNG("m", Path_Font + Font, _content1, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.textures[1] = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        delete[] content;
    }
    if (_content2 != "")
    {
        content = Font::Load_FontToPNG("m", Path_Font + Font, _content2, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.textures[2] = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        delete[] content;
    }
    if (_content3 != "")
    {
        content = Font::Load_FontToPNG("m", Path_Font + Font, _content3, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.textures[3] = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        delete[] content;
    }
    if (_content4 != "")
    {
        content = Font::Load_FontToPNG("m", Path_Font + Font, _content4, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
        _f.textures[4] = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
        delete[] content;
    }
    content = Font::Load_FontToPNG("m", Path_Font + Font, _content, _pixel, _lwh.x, _lwh.y, &_f.length);//msyh.ttc //DejaVuSansMono.ttf
    _f.texture = Load_texture_image(content, _f.length, _lwh.x, _lwh.y, GL_RGBA);
    delete[] content;
    _f.content = NULL;
    _f.mod = _mod;
    _f.function = _function;
    return _f;
}


class UI_Analysis
{
private:
    static UI_Font font;
public:
    UI_Analysis()
    {
    
    }

    ~UI_Analysis()
    {

    }

    static UI_Font Get(string _path, void* _function)
    {
        //���ж�ȡ
        ifstream infile;
        infile.open(_path.data());   //���ļ����������ļ��������� 
        assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

        string s = "";
        bool active_wirte = false;
        while (getline(infile, s))
        {
            s = UTF8ToGB(s.c_str());
            //count << s << endl;
            if (active_wirte)
            {
                Print::Debug("UI_Font" + s);
                if (Data(s))
                {
                    active_wirte = false;
                }
            }
            else if (Line(s))
            {
                active_wirte = true;
            }
        }
        infile.clear();
        infile.close();             //�ر��ļ������� 
        font.function = _function;
        return font;
    }
    static bool Data(string _s)
    {
        if (_s.length() < 2)
            return false;
        if (_s.substr(0, 2) == "Po")
        {
            font.point = StringToVec3(_s.substr(2 + 1));
        }
        else if (_s.substr(0, 2) == "LW")
        {
            font.lwh = StringToVec3(_s.substr(2 + 1));
        }
        else if (_s.substr(0, 2) == "Tx")
        {
            Load_texture(&font.texture, _s.substr(2 + 1).c_str());
        }
        else if (_s.substr(0, 2) == "Fo")
        {
            string* _font_content = new string(_s.substr(2 + 1));
            font.content = Font::Load_FontToPNG("m", Path_Font + "simfang.ttf", *_font_content, 32, font.lwh.x, font.lwh.y, &font.length);//msyh.ttc //DejaVuSansMono.ttf
            font.texture = Load_texture_image(font.content, font.length, font.lwh.x, font.lwh.y, GL_RGBA);
        }
        return false;
    }
    static bool Line(string _s)
    {
        if (_s == "#")
        {
            return true;
        }
        return false;
    }
};
UI_Font UI_Analysis::font;
