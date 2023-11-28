/*{
    23. В текстовом файле записан  отдельный  абзац.  Некоторые
    слова перенесены со строки на следующую строку.  Знак переноса
    '-'.  Выровнять строки абзаца по ширине. Иными словами, правые
    границы  строк  выравниваются  по  заданной  позиции  за  счет
    вставки дополнительных пробелов между словами.  Первая  строка
    абзаца должна иметь заданный отступ, а остальные строки должны
    начинаться  с  первой  позиции.  Последняя  строка  абзаца  по
    правому  краю  не  выравнивается. Переносы в выходном файле не
    допускаются.  Число  строк в исходном и сконечном файлах может
    отличаться (9).

    Назаров Максим Дмитриевич Пс-21

    C++

    -
}*/

#include <iostream>
#include <fstream>
#include <string>

void alignText(const std::string &inputFileName, const std::string &outputFileName, int lineWidth)
{
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    int numberWords = 0;
    std::string line = "   ";
    std::string word;
    std::string some;
    bool checkF = true;

    while (inputFile >> word)
    {
        if (word[word.length() - 1] == '-')
        {
            inputFile >> some;
            word.erase(word.size() - 1);
            word = word + some;
        }
        if ((line.length() + word.length() + 1) <= lineWidth)
        {
            if (!checkF)
            {
                line += ' ';
            }
            checkF = false;
            line += word;
            if (word[word.length() - 1] == '-')
            {
                inputFile >> word;
                word.erase(word.size() - 1);
                line += word;
                numberWords++;
            }
            numberWords++;
        }
        else
        {
            int extraSpaces = lineWidth - line.length();
            int spaceCount;
            int spaceCountAdditional;
            spaceCount = numberWords > 1 ? extraSpaces / (numberWords - 1) : 0;
            spaceCountAdditional = numberWords > 2 ? extraSpaces % (numberWords - 1) : 0;

            int foo;
            bool check = false;
            for (size_t i = 0; i < line.length(); i++)
            {
                if (!check)
                {
                    if (line[i] != ' ')
                    {
                        check = true;
                    }
                }
                outputFile << line[i];
                if (line[i] == ' ' && check)
                {
                    for (int j = 1; j <= spaceCount; j++)
                    {
                        outputFile << ' ';
                    }
                    if (spaceCountAdditional > 0)
                    {
                        outputFile << ' ';
                        spaceCountAdditional--;
                    }
                }
            }

            outputFile << '\n';
            line = word;
            if (word[word.length() - 1] == '-')
            {
                inputFile >> word;
                line.erase(line.size() - 1);
                line += word;
                numberWords++;
            }
            numberWords = 1;
        }
    }

    outputFile << line << '\n';

    inputFile.close();
    outputFile.close();

    std::cout << "File successfully processed." << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " input_filename output_filename line_width" << std::endl;
        return 1;
    }

    const std::string inputFileName = argv[1];
    const std::string outputFileName = argv[2];
    int lineWidth = std::stoi(argv[3]);
    alignText(inputFileName, outputFileName, lineWidth);

    return 0;
}