#ifndef CORE_FILE_READER_H
#define CORE_FILE_READER_H

#include "exception.h"

class FileReader
{
    private:
        std::string file_path;
        std::ifstream ifs;
        uint64_t file_size;

    public:
        enum SeekPosition
        {
            START,   /*!< Start of the audio data */
            CURRENT, /*!< Current position on the audio data */
            END      /*!< End of audio data */
        };

        FileReader(const FileReader &) = delete;
        FileReader & operator=(const FileReader &) = delete;

        /*!
        Open an existing file.
        \param[in] file        Input file path.
        \param[in] seek_to_end Insert the pointer to the end of the file instead of the start.
        \throw ReadFileException
        \throw SeekException
        */
        FileReader(const char * const file, bool seek_to_end);

        /*!
        Get the file size in bytes.
        */
        uint64_t GetFileLength(void) const noexcept;

        /*!
        Seek to \c position relative to \c offset.
        \param[in] position Refer to \ref SeekPosition.
        \param[in] offset   An offset relative to \c position.
        \return offset relative to the start of the file.
        \throw SeekException
        */
        uint64_t Seek(SeekPosition position, int64_t offset);

        /*!
        Get the offset relative to the start of the file.
        \return offset relative to the start of the file.
        \throw SeekException
        */
        uint64_t Tell(void);

        /*!
        Read some data on the file.
        \param[out] buffer           Valid data read will be filled here.
        \param[in]  valid_byte_count Size of data to be read.
        \return Valid number of bytes read.
        \throw ReadFileException
        \note <tt>read_count == 0</tt> will not cause an exception to be thrown.
        \note End-of-file will cause an exception to be thrown.
        */
        uint32_t Read(char *buffer, uint32_t valid_byte_count);

        /*!
        Close the file.
        */
        ~FileReader(void);
};

#endif