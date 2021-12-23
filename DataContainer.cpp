#include "DataContainer.h"
#include "crc.h"

TLMPacket::TLMPacket() : m_header(nullptr)
{
}

void TLMPacket::addFrame(QByteArray& in)
{
    for (auto& mem : in)
    {
        m_data.push_back(mem);
    }
}

void TLMPacket::makeUsefulData()
{
    tsPacket* pPackage = (tsPacket*)m_data.data();

    b2l(pPackage->header, HEADER_SIZE/2);
    b2l(&pPackage->crc, CRC_SIZE/2);

    m_header = (tsHeader*)pPackage->header;

    if (crc_calc(pPackage->data, m_header->packetDataLen) == pPackage->crc)
        m_crc = QString("CRC OK:\n%1").arg(QString().number(pPackage->crc, 16).prepend("0x"));
    else
        m_crc = QString("BAD CRC:\n%1").arg(QString().number(pPackage->crc, 16).prepend("0x"));

    if (m_data.size() - (SYNCHRO_MARKER_SIZE + HEADER_SIZE + CRC_SIZE) == m_header->packetDataLen)
    {
        std::vector< uint8_t>::iterator it = m_data.begin() + (SYNCHRO_MARKER_SIZE + HEADER_SIZE);
        while (it != m_data.end() - CRC_SIZE)
        {
            uint16_t* type = (uint16_t*)&*it;
            it += IDENTIFIER_SIZE;
            if (*type == TM_OBTS)
            {
                tsOBTS* pObts = (tsOBTS*)&*it;
                m_obts.push_back(*pObts);
                it += OBTS_SIZE;
            }
            else if (*type == TM_TEXT)
            {
                tsTextMessage* pMes = (tsTextMessage*)&*it;
                m_textMessage.push_back(*pMes);
                it += TEXT_MESSAGE_SIZE;
            }
        }
    }
}

void TLMPacket::b2l(uint16_t* in, int len)
{
    for (int i = 0; i < len; i++)
    {
        uint8_t byte1 = (in[i] >> 8) & 0xFF;
        uint8_t byte2 = in[i] & 0xFF;
        in[i] = (byte2 << 8) + byte1;
    }
}

void TLMPacket::allClear()
{
    m_data.clear();
    m_obts.clear();
    m_textMessage.clear();
}

Output::Output(int in_id, QString in_tlm, QString in_type,
               int in_len, QString in_crc, int in_packVer,
               int in_appIdent) :
        m_info(in_tlm), m_idPackage(in_id), m_type(in_type),
        m_packLen(in_len), m_crc(in_crc), m_packVer(in_packVer),
        m_appIdent(in_appIdent)
{
}

Output::Output(int in_id, QString&& in_tlm, QString&& in_type,
               int in_len, QString&& in_crc, int in_packVer,
               int in_appIdent) :
        m_info(in_tlm), m_idPackage(in_id), m_type(in_type),
        m_packLen(in_len), m_crc(in_crc), m_packVer(in_packVer),
        m_appIdent(in_appIdent)
{
}


QString Output::getByte() const
{
    return m_info;
}

int Output::getId() const
{
    return m_idPackage;
}

QString Output::getType() const
{
    return m_type;
}

QString Output::getLen() const
{
    if (m_packLen)
        return QString().number(m_packLen, 10);
    else
        return QString();
}

QString Output::getCrc() const
{
    return m_crc;
}

QString Output::getPackVer() const
{
    if (m_packVer)
        return QString().number(m_packVer, 16).prepend("0x");
    else
        return QString();
}

QString Output::getAppIdent() const
{
    if (m_appIdent)
        return QString().number(m_appIdent, 16).prepend("0x");
    else
        return QString();
}
