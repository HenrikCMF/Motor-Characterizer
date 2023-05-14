#ifndef TELEGRAM_H
#define TELEGRAM_H
#include <stdio.h>
#define GRAPH_TELEGRAM 1
#define MISC_TELEGRAM 2

typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
class Telegram
{
    private:
    BYTE *array;
    int index;
    int size;
    void DWORD_to_BYTE(DWORD input, uint8_t *array);
    public:
    int ID;
    inline Telegram(void)
    {
        ID=0;
        array=NULL;
        size=0;
    };
    Telegram(int id, int size);
    ~Telegram();
    inline BYTE* get_data() //returns a pointer to the array containing all data
    {
        return array;
    }
    inline size_t get_numbytes(void) //returns number of bytes in array
    {
        return (size_t)index;
    }
    bool append_DWORD(DWORD input); //convert DWORD to byte and store in the array
    
};

#endif