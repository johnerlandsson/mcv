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
        //Enum containing the order of visible columns in the alarm list
        enum Columns
        {
            Timestamp = 0,
            Message = 1,
            Repeat = 2
        };

        //Enum containing the id's of the alarm_types table
        enum AlarmTypes
        {
            BarcodeTimeout = 1,
            InvalidBarcode = 2,
            MissingHole = 3,
            InvalidProfile = 4,
            NoHolesInProfile = 5
        };

        /* Default constructor
         */
        AlarmsTableModel();

        /* Default destructor
         */
        virtual ~AlarmsTableModel();

        /* \brief Create all tables in database if they do not exist already.
         */
        void createTables();

        /* \brief Implementation of QAbstractTableModel's virtual function
         * @param parent Not used.
         * \return The number of rows of model data.
         */
        int rowCount( const QModelIndex &parent = QModelIndex() ) const;

        /* \brief Implementation of QAbstractTableModel's virtual function
         * @param parent Not used.
         * \return The number of colums of model data.
         */
        int columnCount( const QModelIndex &parent = QModelIndex() ) const;

        /* \brief Implementation of QAbstractTableModel's virtual function
         * @param index Index from wich to return the data
         * $role Should always be Qt::DisplayRole
         * \return Data at specific index. Returns QVariant() if index is invalid.
         */
        QVariant data( const QModelIndex &index, int role ) const;

        /* \brief Implementation of QAbstractTableModel's virtual function
         * @param index Index of data to alter.
         * @param value. New value.
         * @param role. Should always be Qt::EditRole
         * \return True if edit was successful.
         */
        bool setData( const QModelIndex &index, const QVariant &value, int role );

        /* \brief Reimplementation of headerData.
         * @param section Position of header item.
         * @param orientation Should always be Qt::Horizontal
         * @param role Should always be Qt::DisplayRole
         * \return Column header
         */
        QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

        /* \brief Enable/Disable specific alarms.
         *
         * A disabled alarm will be completley ignored.
         */
        void setAlarmEnabled( const AlarmTypes type, const bool enabled );

    private slots:
        void loadData();
        void raiseBarcodeTimeoutAlarm();
        void raiseInvalidBarcodeTimeoutAlarm();
        void raiseMissingHoleAlarm();
        void raiseInvalidProfileAlarm();
        void raiseNoHolesInProfileAlarm();

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
        void holeRelatedAlarmRaised();
        void barcodeRelatedAlarmRaised();

    public slots:
};

#endif // ALARMSTABLEMODEL_H
