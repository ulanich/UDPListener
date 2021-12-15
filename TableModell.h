#pragma once
#include <QAbstractTableModel>
#include <QModelIndex>
#include <vector>
#include "DataContainer.h"

#define MAX_LIST_SIZE (10000)

class QTableViewModel : public QAbstractListModel
{
public:   
      QTableViewModel(QObject* parent = nullptr);
      int   rowCount(const QModelIndex&) const;
      int   columnCount(const QModelIndex& parent) const;
      QVariant  data(const QModelIndex& index, int role) const;
      QVariant QTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const;
      void  pushBackRow(QByteArray value);
      void QTableViewModel::deleteRow(int idx);
      void  disableDataFlag();

private:
    QList<Output>* m_values;
    TLMPacket m_tlm;
    bool data_flag;

    void addPack(TLMPacket& value);
    void append(Output& value);
};
