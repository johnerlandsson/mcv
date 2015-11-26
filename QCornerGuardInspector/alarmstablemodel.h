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

        enum AlarmTypes
        {
            BarcodeTimeout = 1,
            InvalidBarcode = 2,
            MissingHole = 3,
            InvalidProfile = 4
        };

        AlarmsTableModel();
        virtual ~AlarmsTableModel();
        void createTables();
        int rowCount( const QModelIndex &parent = QModelIndex() ) const;
        int columnCount( const QModelIndex &parent = QModelIndex() ) const;
        QVariant data( const QModelIndex &index, int role ) const;
        bool setData( const QModelIndex &index, const QVariant &value, int role );
        QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
        void setAlarmEnabled( const AlarmTypes type, const bool enabled );

    private slots:
        void loadData();
        void raiseBarcodeTimeoutAlarm();
        void raiseInvalidBarcodeTimeoutAlarm();
        void raiseMissingHoleAlarm();
        void raiseInvalidProfileAlarm();

    private:
        void incrementLastRepeat();
        void addAlarm(const AlarmTypes type );
        QString AlarmMessage( const AlarmTypes type ) const;

        QList<QDateTime> timestamps;
        QList<int> repeats;
        QList<int> ids;
        QList<int> typeids;
        QSqlDatabase db;
        bool barcodeTimeoutEnabled;
        bool invalidBarcodeEnabled;
        bool missingHoleEnabled;
        bool invalidProfileEnabled;

    signals:

    public slots:
};

#endif // ALARMSTABLEMODEL_H
