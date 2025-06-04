#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include <cstdint>
#include <cstring>

class Stream {
public:
    virtual ~Stream() {}
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t*, size_t) = 0;
    virtual int read() = 0;
    virtual int available() = 0;
    virtual void flush() = 0;
    virtual int peek() = 0;
};

class MemStream : public Stream {
private:
    uint8_t* _buffer;
    const uint16_t _len;
    bool _buffer_overflow;
    uint16_t _pos_read;
    uint16_t _pos_write;
    bool _allowWrite;

public:
    MemStream(uint8_t* buffer, const uint16_t len, uint16_t content_len = 0, bool allowWrite = true)
        : _buffer(buffer), _len(len), _buffer_overflow(false), _pos_read(0), _pos_write(content_len), _allowWrite(allowWrite) {
        if (content_len == 0) std::memset(_buffer, 0, _len);
        if (content_len > len) {
            content_len = len;
            _pos_write = len;
        }
    }

    ~MemStream() {}

    operator const uint8_t*() const { return _buffer; }
    operator const char*() const { return (const char*)_buffer; }

    uint16_t current_length() const { return _pos_write; }

    bool overflow() { 
        bool ret = _buffer_overflow; 
        _buffer_overflow = false; 
        return ret; 
    }

    int peek() override {
        if (_pos_read >= _len) {
            _buffer_overflow = true;
            return -1;
        }
        if (_pos_read >= _pos_write) {
            return -1;
        }
        return _buffer[_pos_read + 1];
    }

    size_t write(uint8_t b) override {
        if (!_allowWrite) return -1;
        if (_pos_write >= _len) {
            _buffer_overflow = true;
            return 0;
        }
        _buffer[_pos_write] = b;
        ++_pos_write;
        return 1;
    }

    size_t write(const uint8_t* buf, size_t size) override {
        if (!_allowWrite) return -1;
        if (_pos_write + size > _len) {
            _buffer_overflow = true;
            return 0;
        }
        std::memcpy(_buffer + _pos_write, buf, size);
        _pos_write += size;
        return size;
    }

    int read() override {
        if (_pos_read >= _len) {
            _buffer_overflow = true;
            return -1;
        }
        if (_pos_read >= _pos_write) {
            return -1;
        }
        return _buffer[_pos_read++];
    }

    int available() override {
        int ret = _pos_write - _pos_read;
        if (ret < 0) ret = 0;
        return ret;
    }

    void flush() override {
        std::memset(_buffer, 0, _len);
        _pos_write = 0;
        _pos_read = 0;
    }

    void setBufferContent(uint8_t* buffer, uint16_t content_len) {
        std::memset(_buffer, 0, _len);
        std::memcpy(_buffer, buffer, content_len);
        _buffer_overflow = false;
        _pos_write = content_len;
        _pos_read = 0;
    }

    void setBufferContentPosition(uint16_t read_pos, uint16_t write_pos) {
        _pos_write = write_pos;
        _pos_read = read_pos;
    }
};

#endif