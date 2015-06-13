#ifndef CHATLEXICSCUTTER_H
#define CHATLEXICSCUTTER_H

typedef std::vector< std::string > LC_AnalogVector;
typedef std::map< std::string, LC_AnalogVector > LC_AnalogMap;
typedef std::set< std::string > LC_LetterSet;
typedef std::vector< LC_LetterSet > LC_WordVector;
typedef std::vector< LC_WordVector > LC_WordList;
typedef std::multimap< std::string, unsigned int > LC_WordMap;

static int trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

class LexicsCutter
{
    protected:
        LC_AnalogMap AnalogMap;
        LC_WordList WordList;
        LC_WordMap WordMap;

        std::string InvalidChars;

    public:
        LexicsCutter();

        static bool ReadUTF8(std::string& in, std::string& out, unsigned int& pos);

        std::string trim(std::string& s, const std::string& drop = " ");
        static std::string ltrim(std::string &data);
        bool ReadLetterAnalogs(std::string& FileName);
        bool ReadInnormativeWords(std::string& FileName);
        void MapInnormativeWords();
        bool CompareWord(std::string& str, unsigned int pos, LC_WordVector word);
        bool CheckLexics(std::string& Phrase);
        
        std::vector< std::pair< unsigned int, unsigned int > > Found;
        bool IgnoreMiddleSpaces;
        bool IgnoreLetterRepeat;
        bool CheckLetterContains;
};

#endif
