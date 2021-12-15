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
        m_crc = QString("CRC OK: %1").arg(QString().number(pPackage->crc, 16).prepend("0x"));
    else
        m_crc = QString("BAD CRC: %1").arg(QString().number(pPackage->crc, 16).prepend("0x"));

    if (m_data.size() - (SYNCHRO_MARKER_SIZE + HEADER_SIZE + CRC_SIZE) == m_header->packetDataLen)
    {
        std::vector< uint8_t>::iterator it = m_data.begin() + (SYNCHRO_MARKER_SIZE + HEADER_SIZE);
        while (it != m_data.end() - CRC_SIZE)
        {
            uint16_t* type = (uint16_t*)&*it;
            it += 2;
            if (*type == TM_OBTS)
            {
                tsOBTS* buf = (tsOBTS*)&*it;
                m_obts.push_back(*buf);
                it += 4;
            }
            else if (*type == TM_TEXT)
            {
                tsTextMessage* buf1 = (tsTextMessage*)&*it;
                m_textMessage.push_back(*buf1);
                it += 16;
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
    return m_packLen;
}

QString Output::getCrc() const
{
    return m_crc;
}