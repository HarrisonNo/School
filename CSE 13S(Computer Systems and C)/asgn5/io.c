#include "io.h"
#include <unistd.h>
#include <fcntl.h>

uint64_t bytes_read = 0; //Represents TOTAL bytes read
uint64_t bytes_written = 0;
static uint8_t WriteBuffer[BLOCK] = { 0 }; //pdf recommends this buffer be static to whole file
static int TotalIndex = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int MostRecentNumberOfBytesRead = 0, BytesReadThisFunction = 0;
    if (nbytes == 0) {
        return 0;
    }
    do {
        MostRecentNumberOfBytesRead = read(infile, buf + bytes_read, nbytes - bytes_read);
        bytes_read += MostRecentNumberOfBytesRead;
        BytesReadThisFunction += MostRecentNumberOfBytesRead;
    } while (BytesReadThisFunction < nbytes && MostRecentNumberOfBytesRead != 0);
    return BytesReadThisFunction;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int MostRecentNumberOfBytesWritten = 0, BytesWrittenThisFunction = 0;
    if (nbytes == 0) {
        return 0;
    }
    do {
        MostRecentNumberOfBytesWritten
            = write(outfile, buf + bytes_written, nbytes - bytes_written);
        bytes_written += MostRecentNumberOfBytesWritten;
        BytesWrittenThisFunction += MostRecentNumberOfBytesWritten;
    } while (BytesWrittenThisFunction < nbytes && MostRecentNumberOfBytesWritten != 0);
    return BytesWrittenThisFunction;
}

bool read_bit(int infile, uint8_t *bit) //Credit to TA Christian
{
    static uint8_t ReadBuffer[BLOCK] = { 0 };
    static uint32_t TopBit = 0;
    static uint32_t BitBufferSize = 0;

    if (BitBufferSize == 0) {
        BitBufferSize
            = read_bytes(infile, &ReadBuffer[0], BLOCK); //Dont know if correct parameters!!!
        if (BitBufferSize == 0) {
            return false; //failed
        }
    }
    *bit = ReadBuffer[TopBit];
    TopBit++;
    if (TopBit == BitBufferSize * 8) //Reset
    {
        TopBit = 0;
        BitBufferSize = 0;
    }
    return true;
}

void write_code(int outfile, Code *c) //Credit to Eugene
{
    for (uint32_t i = 0; i < code_size(c); i++) {
        //loops until 'i' is out of reach of the code
        WriteBuffer[TotalIndex] = code_get_bit(c, i);
        TotalIndex++;
        if (TotalIndex == BLOCK) //only write once buffer is full
        {
            flush_codes(outfile);
        }
    }
    return;
}

void flush_codes(int outfile) //Credit to Eugene
{
    int NumberOfBytes = 0;
    if (TotalIndex <= 0) //just immediately exit if 0, or for whatever reason less than zero
    {
        return;
    }
    NumberOfBytes = (int) TotalIndex / 8; //effectively results in rounding down
    if (TotalIndex % 8
        != 0) //if there is some remainder then add 1 to number of bytes to effectively round up, could also just include math.h but might as well keep the num of includes to a min
    {
        NumberOfBytes++;
    }
    write_bytes(outfile, &WriteBuffer[0], NumberOfBytes);
    TotalIndex = 0;
    for (int i = 0; i < TotalIndex;
         i++) //Reset contents of WriteBuffer, may not necessarily be needed
    {
        WriteBuffer[i] = 0;
    }
}
