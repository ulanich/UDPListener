#pragma once
#include <QString>
#include <vector>
#include "InfoStructs.h"

class Output
{
public:
    Output(int in_id = 0, QString& in_tlm = QString(),
        QString& in_type = QString(), int in_len = 0,
        QString& in_crc = QString(), int in_packVer = 0,
        int in_appIdent = 0);

    QString    getByte() const;
    int        getId() const;
    QString    getType() const;
    QString    getLen() const;
    QString    getCrc() const;
    QString    getPackVer() const;
    QString    getAppIdent() const;

private:
    QString    m_info;
    int        m_idPackage;
    QString    m_type;
    int        m_packLen;
    QString    m_crc;
    int        m_packVer;
    int        m_appIdent;
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
