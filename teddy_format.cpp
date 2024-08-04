#include <iostream>
#include <fstream>
#include <windows.h>
#include <tchar.h>
#include <regex>

using namespace std;

void UTF8ToUnicode(wchar_t* szUni, const char* szUtf) // 把 UTF8 轉成 Unicode
{
    MultiByteToWideChar(CP_UTF8, 0, szUtf, -1, szUni, (int)strlen(szUtf) * 2);
}

void PrintfUnicode(const wchar_t* szFormat, ...) // 在 console 印出 Unicode
{
    const int MAX_PRINT_NUM = 1024;
    wchar_t szPrint[MAX_PRINT_NUM] = L"";
    va_list pArgs;
    va_start(pArgs, szFormat);
    vswprintf_s(szPrint, szFormat, pArgs);
    va_end(pArgs);

    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), szPrint, (DWORD)wcslen(szPrint), NULL, NULL);
}

int main()
{
    ifstream file("input.txt");
    string line;

    if (!file.is_open())
    {
        cout << "Failed to open file." << endl;
        system("pause");
        return 0;
    }
    //regex englishRegex2("[a-z][.]");
    //regex eg_pattern2("\\s{4}[a-z].\\s"); // a. b. c.



    //regex numRegex("[0-9]+[.]");
    //regex num_pattern("\\s{2}[0-9]+[.]\\s"); // 1. 2. 3.

    while (getline(file, line))
    {

        //while (regex_search(templine, match, englishRegex)) {
        //    cout << match.str()<<endl;
        //    if (!regex_search(match.str(), eg_pattern1)) {
        //        hasError = true;
        //    }
        //    /*for (auto x : match) std::cout << x << " ";
        //    std::cout << std::endl;*/
        //    templine = " " + match.suffix().str();
        //
        //if (regex_search(line, englishRegex))
        //{
        //    // 檢查前後是否有加上空白
        //    // 使用 regex_search 在每一行中搜索所有匹配项
        //    smatch match;
        //    size_t position = 0;
        //    string templine = line;
        //    cout << match.position() << endl;;
        //    while (regex_search(templine, match, eg_pattern1))
        //    {
        //        // 如果找到匹配项，则表示格式正确
        //        position = match.position() + 1;
        //        for (auto x : match) std::cout << x << " ";
        //        std::cout << std::endl;
        //        templine = " "+match.suffix().str();
        //        hasError = true;
        //    }
        //    cout << match.position()<<endl;

            //// 如果position不在字串最後，代表找到錯誤的字
            //if (position < templine.size()) {
            //    hasError = true;
            //}
            
            
            
            //if (!(regex_search(line, eg_pattern1)))// && !isalpha(line[0])))
            //{
            //    //cout << regex_search(line, eg_pattern1) << endl;
            //    //cout << regex_search(line, eg_pattern2) << endl;

            //    hasError = true;
            //}
            // a. b. c.
            //if (regex_search(line, englishRegex2))
            //{
            //    //cout << regex_search(line, eg_pattern1) << endl;
            //    //cout << regex_search(line, eg_pattern2) << endl;
            //    if (!regex_search(line, englishRegex2))
            //        hasError = true;
            //}
            
        
    /*    if (regex_search(line, numRegex)) {
            if (!(regex_search(line, num_pattern)))
            {
                hasError = true;
            }
        }*/

        bool hasError = false;
        // 檢查是否有英文字
        // 使用 regex_search 在每一行中搜索所有匹配项
        smatch match;
        size_t position = 0;
        string templine = line;
        //regex englishRegex("\\s[a-zA-Z]+");
        regex englishRegex("[^a-zA-Z]?[*.(]*[a-zA-Z]+[)]*[^a-zA-Z]?");
        regex eg_pattern1("\\s[*.(]*[a-zA-Z]+[)]*\\s"); // english words with space
        regex abcRegex("[^a-zA-Z]{0,4}[a-z][.][^a-zA-Z]"); // a. b. c.
        regex abc_pattern("\\s{4}[a-z][.]\\s"); // a. b. c.
        regex numRegex("[^0-9]{0,2}[0-9]+[.][^0-9]"); // a. b. c.
        regex num_pattern("\\s{2}[0-9]+[.]\\s"); // a. b. c.

        // 轉換成 Unicode 並在 console 印出 Unicode
        wchar_t szUnicode[512] = L"";
        UTF8ToUnicode(szUnicode, line.c_str());
        PrintfUnicode(L"%s\n", szUnicode);

        if (regex_search(templine, match, numRegex)) {
            if (!regex_search(templine, match, num_pattern)) {
                UTF8ToUnicode(szUnicode, "123 error");
                PrintfUnicode(L"\033[1;31m%s\033[0m\n", szUnicode); // 以紅色字體輸出錯誤行
            }
            templine = " " + match.suffix().str();

        }
        if (regex_search(templine, match, abcRegex)) {
            if (!regex_search(templine, match, abc_pattern)) {
                UTF8ToUnicode(szUnicode, "abc error");
                PrintfUnicode(L"\033[1;31m%s\033[0m\n", szUnicode); // 以紅色字體輸出錯誤行
            }
            templine = " " + match.suffix().str();

        }
        while (regex_search(templine, match, englishRegex)) {
            //cout << match.str() <<"haha" << endl;
            //cout << match.str() << " " << match.position() << endl;
            char first_match = match.str()[0];
            char last_match = match.str()[match.str().length()-1];
            //前後沒空白、句子第一個字不是英文、句子最後一個字不是英文
            if (!regex_search(match.str(), eg_pattern1) 
                && !(first_match >= 97 && first_match <= 122 || first_match >= 65 && first_match <= 90)
                && !(last_match >= 97 && last_match <= 122 || last_match >= 65 && last_match <= 90))
            {
                hasError = true;
                //PrintfUnicode(L"\033[1;31m%s\033[0m\n", szUnicode); // 以紅色字體輸出錯誤行
                UTF8ToUnicode(szUnicode, ("space error: " + match.str()).c_str());
                PrintfUnicode(L"\033[1;31m%s\033[0m\n", szUnicode); // 以紅色字體輸出錯誤行
            }
            //else {
              //  PrintfUnicode(L"%s\n", szUnicode);
            //}
            /*for (auto x : match) std::cout << x << " ";
            std::cout << std::endl;*/
            templine = " " + match.suffix().str();
            //cout << templine << endl;
        }
        //if(regex_search(templine, match, englishRegex))
        ////if (regex_match(line, match ,englishRegex))
        //{
        //    cout << match.str()<<" " << match.position() <<endl;
        //    PrintfUnicode(L"%s\n", szUnicode);
        //    cout << endl;
        //}
        //else
        //{
        //    
        //    PrintfUnicode(L"\033[1;31m%s\033[0m\n", szUnicode); // 以紅色字體輸出錯誤行
        //}
    }

    file.close();

    system("pause");
    return 0;
}
