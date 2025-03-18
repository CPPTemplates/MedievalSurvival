#include <SFML/System/InputStream.hpp>
#include <memory>
#include <fstream>
#pragma once

inline std::size_t getStreamSize(std::istream& stream){
    
        const std::streampos& curr = stream.tellg();

        stream.seekg(0, stream.end);
        const std::streampos& pos1 = stream.tellg();

        stream.seekg(curr, stream.beg);

        return (std::size_t)pos1;
}

// https://stackoverflow.com/questions/28312737/c-implementing-sfml-inputstream
class sfmlInputStream final : public sf::InputStream
{
    std::shared_ptr<std::istream> m_source;
    const size_t size;

public:
    explicit sfmlInputStream(std::shared_ptr<std::istream> stream) : m_source(stream), size(getStreamSize(*stream)) {}

    std::optional<std::size_t> read(void *data, std::size_t size) final
    {
        if(!m_source->good()){
            return -1;
        }
        const size_t& maximumReadableSize = this->size - (size_t)m_source->tellg();
        const size_t& max = math::minimum(size, maximumReadableSize);
        m_source->read(static_cast<char *>(data), max);

        // return m_source->gcount();
        return m_source->good() ? max : -1;
    }
    std::optional<std::size_t> seek(std::size_t position) final
    {
        const size_t& max = math::minimum(position, this->size);
        m_source->seekg(max, m_source->beg);

        return m_source->good() ? max : -1;
        // return m_source->gcount();
        // return m_source->good()? m_source->gcount() : -1;
    }
    virtual std::optional<std::size_t> tell() final
    {
        std::streampos pos = m_source->tellg();
        return pos;
    }
    virtual std::optional<std::size_t> getSize() final
    {
        return size;
    }
};