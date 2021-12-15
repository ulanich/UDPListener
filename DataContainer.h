#pragma once
#include <QString>
#include <vector>
#include "InfoStructs.h"

class Output
{
public:
    Output(int in_id = 0, QString& in_tlm = QString(),
        QString& in_type = QString(), QString& in_len = QString(),
        QString& in_crc = QString()) :
        m_idPackage(in_id), m_info(in_tlm), m_type(in_type),
        m_packLen(in_len), m_crc(in_crc)
    {
    }

    QString    getByte() const;
    int        getId() const;
    QString    getType() const;
    QString    getLen() const;
    QString    getCrc() const;

private:
    QString    m_info;
    int        m_idPackage;
    QString    m_type;
    QString    m_packLen;
    QString    m_crc;
};

class TLMPacket
{
public:
    TLMPacket();
    void    addFrame(QByteArray& in);
    void    makeUsefulData();
    void    b2l(uint16_t* in, int len);
    void    allClear();

    std::vector<uint8_t>        m_data;
    std::vector<tsTextMessage>  m_textMessage;
    std::vector<tsOBTS>         m_obts;
    Output                      m_outputData;
    QString                     m_crc;
    tsHeader*                   m_header;
};
