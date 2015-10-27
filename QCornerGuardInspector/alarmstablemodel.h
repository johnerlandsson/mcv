#ifndef ALARMSTABLEMODEL_H
#define ALARMSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDateTime>
#include <QSqlDatabase>

class AlarmsTableModel : public QAbstractTableModel
{
    Q_OBJECT
    public:
        enum Columns
        {
            Timestamp = 0,
            Message = 1,
            Repeat = 2
        };

        AlarmsTableModel();
        virtual ~AlarmsTableModel();
        void createTables();
        int rowCount( const QModelIndex &parent = QModelIndex() ) const;
        int columnCount( const QModelIndex &parent = QModelIndex() ) const;
        QVariant data( const QModelIndex &index, int role ) const;
        bool setData( const QModelIndex &index, const QVariant &value, int role );
        QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    private slots:
        void loadData();

    private:
        QList<QDateTime> timestamps;
        QStringList messages;
        QList<int> repeats;
        QList<int> ids;
        QSqlDatabase db;

    signals:

    public slots:
};

#endif // ALARMSTABLEMODEL_H