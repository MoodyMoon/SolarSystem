#include "stdafx.h"
#include "file_reader.h"

FileReader::FileReader(const char * const file, bool seek_to_end) : file_path(file)
{
    std::ifstream::openmode openmode = std::ifstream::in | std::ifstream::binary | std::ifstream::ate;

    ifs.open(file, openmode);

    if(ifs.fail())
    {
        std::string error_message("Cannot open \"");
        error_message.append(file);
        error_message.append("\".");
        throw ReadFileException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    file_size = ifs.tellg();
    if(ifs.fail())
    {
        std::string error_message("Cannot get file size of \"");
        error_message.append(file);
        error_message.append("\".");
        throw SeekException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    if(!seek_to_end)
    {
        ifs.seekg(0, std::ifstream::beg);
        if(ifs.fail())
        {
            std::string error_message("Cannot perform seeking on \"");
            error_message.append(file);
            error_message.append("\".");
            throw SeekException("FileReader", ifs.rdstate(), error_message.c_str());
        }
    }
}

uint64_t FileReader::GetFileLength() const noexcept
{
    return file_size;
}

uint64_t FileReader::Seek(SeekPosition position, int64_t offset)
{
    std::ifstream::seekdir way;

    switch(position)
    {
        case START:
            way = std::ifstream::beg;
            break;
        case CURRENT:
            way = std::ifstream::cur;
            break;
        default:
            way = std::ifstream::end;
    }

    ifs.seekg(offset, way);
    if(ifs.fail())
    {
        std::string error_message("Cannot perform seeking on \"");
        error_message.append(file_path);
        error_message.append("\".");
        throw SeekException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    std::streampos pos = ifs.tellg();
    if(ifs.fail())
    {
        std::string error_message("Cannot get the current seek position on \"");
        error_message.append(file_path);
        error_message.append("\".");
        throw SeekException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    return pos;
}

uint64_t FileReader::Tell(void)
{
    std::streampos pos = ifs.tellg();
    if(ifs.fail())
    {
        std::string error_message("Cannot get the current seek position on \"");
        error_message.append(file_path);
        error_message.append("\".");
        throw SeekException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    return pos;
}

uint32_t FileReader::Read(char *buffer, uint32_t valid_byte_count)
{
    ifs.read(buffer, valid_byte_count);
    if(ifs.fail())
    {
        std::string error_message("Cannot read \"");
        error_message.append(file_path);
        error_message.append("\".");
        throw ReadFileException("FileReader", ifs.rdstate(), error_message.c_str());
    }

    return static_cast<uint32_t>(ifs.gcount());
}

FileReader::~FileReader()
{
    ifs.close();
}