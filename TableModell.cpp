#include "TableModell.h"
#include <QModelIndex>
#include <QDebug>

QTableViewModel::QTableViewModel(QObject* parent)
    :QAbstractListModel(parent)
{
    data_flag = false;
    m_values = new QList<Output>();
}

int QTableViewModel::rowCount(const QModelIndex&) const
{
    return m_values->count();
}

int QTableViewModel::columnCount(const QModelIndex&) const
{
    return 7;
}

void QTableViewModel::disableDataFlag()
{
    m_tlm.allClear();
    data_flag = false;
}

QVariant QTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("PACKET COUNTER");
        case 1:
            return QString("PACKET DATA LENGTH");
        case 2:
            return QString("INFO");
        case 3:
            return QString("TYPE");
        case 4:
            return QString("CRC");
        case 5:
            return QString("PACKET VERSION");
        case 6:
            return QString("APP IDENTIFIER");
        }
    }
    return QVariant();
}

QVariant QTableViewModel::data(const QModelIndex& index, int role) const
{
    QVariant value;
    
    switch (role)
    {
        case Qt::DisplayRole:
        {
            switch (index.column()) 
            {
                case 0: 
                {
                    value = m_values->at(index.row()).getId();
                    break;
                }
                case 1:
                {
                    value = m_values->at(index.row()).getLen();
                    break;
                }
                case 2: 
                {
                    value = m_values->at(index.row()).getByte();
                    break;
                }
                case 3:
                {
                    value = m_values->at(index.row()).getType();
                    break;
                }
                case 4:
                {
                    value = m_values->at(index.row()).getCrc();
                    break;
                }
                case 5:
                {
                    value = m_values->at(index.row()).getPackVer();
                    break;
                }
                case 6:
                {
                    value = m_values->at(index.row()).getAppIdent();
                    break;
                }
            }
            break;
        }

        case Qt::TextAlignmentRole:
        {
            switch (index.column())
            {
                case 0:
                    return Qt::AlignCenter;
                case 1:
                    return Qt::AlignCenter;
                case 2:
                    return Qt::AlignLeft;
                case 3:
                    return Qt::AlignCenter;
                case 4:
                    return Qt::AlignCenter;
                case 5:
                    return Qt::AlignCenter;
                case 6:
                    return Qt::AlignCenter;
            }
            break;
        }
        default:
            break;
    }

    return value;
}

void QTableViewModel::append(Output& value)
{
    if (m_values->size() >= MAX_LIST_SIZE) deleteRow(0);
    beginInsertRows(QModelIndex(), m_values->count(), m_values->count());
    m_values->append(value);
    endInsertRows();
}

void QTableViewModel::addPack(TLMPacket& value)
{
    QString usefuldata;
    std::vector<uint8_t>::iterator it = value.m_data.begin() + (SYNCHRO_MARKER_SIZE + HEADER_SIZE);

    for (int i = SYNCHRO_MARKER_SIZE + HEADER_SIZE; i < value.m_data.size() - CRC_SIZE; i++)
    {
        if (i == 47) 
            usefuldata.append("\n");
        usefuldata.append(QStringLiteral("%1").arg(value.m_data.at(i), 2, 16, QLatin1Char('0')));
    }

    value.m_outputData = Output(value.m_header->packetCounter,
        usefuldata, QString("Packet Data"), value.m_header->packetDataLen,
        value.m_crc, value.m_header->packetVersion, value.m_header->appProcessInd);

    append(value.m_outputData);

    for (auto& mem : value.m_obts)
    {
        tsOBTS* ptr = &mem;
        value.m_outputData = Output(value.m_header->packetCounter,
            QString("Date: %1:%2:%3  Time: %4:%5:%6").arg(ptr->day).arg(ptr->month).arg(ptr->year+2000).arg(ptr->hour).arg(ptr->minute).arg(ptr->seconds),
            QString("Date Time\n DD:MM:YYYY HH:MM:SS "));
        append(value.m_outputData);
    }
    for (auto& mem : value.m_textMessage)
    {
        tsTextMessage* ptr = &mem;
        value.m_outputData = Output(value.m_header->packetCounter,
            QString("Arg = %1; Sin(Arg) = %2; Cos(Arg) = %3").arg(ptr->arg).arg(ptr->sinArg).arg(ptr->doubleArg),
            QString("Text Message"));
        append(value.m_outputData);
    }
}

void QTableViewModel::pushBackRow(QByteArray value)
{
    uint16_t* marker = (uint16_t*)value.data();
    if (*marker == TM_SYNCHRO)
    {
        if (data_flag)
        {
            m_tlm.makeUsefulData();
            addPack(m_tlm);
            m_tlm.allClear();
        }
        m_tlm.addFrame(value);
        data_flag = true;
    }
    else if(data_flag)
    {
        m_tlm.addFrame(value);
    }
}

void QTableViewModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx, idx);

    (*m_values).removeAt(idx);

    endRemoveRows();
}
