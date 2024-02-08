#pragma once

#include <vector>
#include <memory>
#include <span>

class FSession;

namespace sv {
    class Packet {
        friend FSession;
        using HandlerFunc = TFunction<void(TSharedPtr<FSession>)>;
    public:
        Packet(unsigned short id, int reserve = 1024);
        virtual ~Packet() {};
    public:
        Packet& operator<<(unsigned char data);
        Packet& operator<<(unsigned short data);
        Packet& operator<<(unsigned int data);
        Packet& operator<<(unsigned long data);
        Packet& operator<<(unsigned long long data);
        Packet& operator<<(bool data);
        Packet& operator<<(char data);
        Packet& operator<<(short data);
        Packet& operator<<(int data);
        Packet& operator<<(long data);
        Packet& operator<<(long long data);
        Packet& operator<<(float data);
        Packet& operator<<(double data);
        Packet& operator<<(std::string_view data);

        template<class T>
        Packet& operator<<(std::vector<T> data) {
            *this << static_cast<unsigned short>(data.size());
            for(const T& elem : data)
                *this << elem;
            return *this;
        }
    public:
        Packet& operator>>(long long& data);
        Packet& operator>>(unsigned char& data);
        Packet& operator>>(unsigned short& data);
        Packet& operator>>(unsigned int& data);
        Packet& operator>>(unsigned long& data);
        Packet& operator>>(unsigned long long& data);
        Packet& operator>>(bool& data);
        Packet& operator>>(char& data);
        Packet& operator>>(short& data);
        Packet& operator>>(int& data);
        Packet& operator>>(long& data);
        Packet& operator>>(float& data);
        Packet& operator>>(double& data);
        Packet& operator>>(std::string& data);

        template<class T>
        Packet& operator>>(std::vector<T>& data) {
            unsigned short len;
            *this >> len;
            for(unsigned short i = 0; i < len; ++i) {
                T t;
                *this >> t;
                data.push_back(t);
            }
            return *this;
        }
    protected:
        void virtual write() {};
        void virtual read();
        void finish();
    public:
        unsigned short getId() const { return m_id; }
        unsigned short getSize() const { return m_size; }
    public:
        void parse(std::span<char> buffer);

        template<class T>
        static TSharedPtr<T> parseFrom(std::span<char> buffer)
        {
            auto pk = MakeShared<T>();
            pk->parse(buffer);
            return pk;
        }
        
        std::vector<char>& data();
        void setHandler(const HandlerFunc& handler);
        void executeHandler(const TSharedPtr<FSession>& session) const;
    private:
        HandlerFunc m_handler;
        std::vector<char> m_buffer;
        unsigned short m_id;
        unsigned short m_size;
    };
}