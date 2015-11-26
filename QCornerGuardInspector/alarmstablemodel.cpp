#include "alarmstablemodel.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <iostream>
#include <QSqlError>

AlarmsTableModel::AlarmsTableModel() : barcodeTimeoutEnabled{ true }, invalidBarcodeEnabled{ true }, missingHoleEnabled{ true }, invalidProfileEnabled{ true }
{
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( "QCornerGuardInspector_alarms.db" );
    if( !db.open() )
    {
        QString msg = "Failed to open database.\n" + db.lastError().text();
        QMessageBox::critical( nullptr, "Error", msg );
        return;
    }

    createTables();
    loadData();
}

AlarmsTableModel::~AlarmsTableModel()
{
    db.close();
}

void AlarmsTableModel::createTables()
{
    if( !db.isOpen() )
        return;

    QSqlQuery q;
    bool res = false;
    res = q.exec( "CREATE TABLE IF NOT EXISTS 'alarm_types'\n"
                  "(\n"
                  "	'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
                  "   'msg' TEXT NOT NULL\n"
                  ");" );
    if( !res )
        return;

    res = q.exec( "CREATE TABLE IF NOT EXISTS 'alarms'\n"
                  "(\n"
                  "   'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
                  "   'alarm_types_id' INTEGER NOT NULL,\n"
                  "   'ts' DATETIME NOT NULL DEFAULT(CURRENT_TIMESTAMP),\n"
                  "   'repeat' INTEGER NOT NULL DEFAULT(0),\n"
                  "	  'image_data' BLOB\n"
                  ");" );
    if( !res )
        return;

    res = q.exec( "SELECT * FROM alarm_types WHERE 'id'=1;" );
    if( !res )
        return;

    q.exec( "INSERT INTO 'alarm_types' (id, msg) VALUES(1, 'Barcode timeout.');" );
    q.exec( "INSERT INTO 'alarm_types' (id, msg) VALUES(2, 'Invalid barcode.');" );
    q.exec( "INSERT INTO 'alarm_types' (id, msg) VALUES(3, 'Missing hole.');" );
    q.exec( "INSERT INTO 'alarm_types' (id, msg) VALUES(4, 'Invalid profile.');" );
}

int AlarmsTableModel::rowCount( const QModelIndex & ) const
{
    return timestamps.count();
}

int AlarmsTableModel::columnCount( const QModelIndex & ) const
{
    return 3;
}

QVariant AlarmsTableModel::data( const QModelIndex &index, int role ) const
{
    if( !index.isValid() )
        return QVariant();

    if( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        switch( index.column() )
        {
            case Timestamp:
                return timestamps[index.row()];
            case Message:
                return AlarmMessage( (AlarmTypes)typeids[index.row()] );
            case Repeat:
                return repeats[index.row()];
            default:
                break;
        }
    }

    return QVariant();
}

bool AlarmsTableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( !index.isValid() || role != Qt::EditRole )
        return false;

    switch( index.column() )
    {
        case Timestamp:
            timestamps[index.row()] = value.toDateTime();
            break;
        case Message:
            break;
        case Repeat:
            repeats[index.row()] = value.toInt();
        default:
            return false;
    }

    return true;
}

QVariant AlarmsTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
        return QAbstractTableModel::headerData( section, orientation, role );

    switch( section )
    {
        case Timestamp:
            return "Timestamp";
        case Message:
            return "Message";
        case Repeat:
            return "Repeat";
        default:
            break;
    }

    return QVariant();
}

void AlarmsTableModel::setAlarmEnabled( const AlarmTypes type, const bool enabled )
{
    switch( type )
    {
        case BarcodeTimeout:
            barcodeTimeoutEnabled = enabled;
            break;
        case InvalidBarcode:
            invalidBarcodeEnabled = enabled;
            break;
        case MissingHole:
            missingHoleEnabled = enabled;
            break;
        case InvalidProfile:
            invalidProfileEnabled = enabled;
            break;
        default:
            break;
    }
}

void AlarmsTableModel::loadData()
{
    QSqlQuery q;
    bool res = false;

    res = q.exec( "SELECT 'alarms'.'ts', 'alarms'.'repeat', 'alarms'.'id', 'alarms'.'alarm_types_id'\n"
                  "FROM 'alarms'\n"
                  "ORDER BY 'alarms'.'ts' DESC\n"
                  "LIMIT 20;" );
    if( !res )
        return;

    emit layoutAboutToBeChanged();

    timestamps.clear();
    repeats.clear();
    ids.clear();
    typeids.clear();

    while( q.next() )
    {
        timestamps << q.value( 0 ).toDateTime();
        repeats << q.value( 1 ).toInt();
        ids << q.value( 2 ).toInt();
        typeids << q.value( 3 ).toInt();
    }

    emit layoutChanged();
}

void AlarmsTableModel::raiseBarcodeTimeoutAlarm()
{
    if( !barcodeTimeoutEnabled )
        return;

    if( ids.size() > 0 && typeids.front() == BarcodeTimeout )
        incrementLastRepeat();
    else
        addAlarm( BarcodeTimeout );
}

void AlarmsTableModel::raiseInvalidBarcodeTimeoutAlarm()
{
    if( !invalidBarcodeEnabled )
        return;

    if( ids.size() > 0 && typeids.front() == InvalidBarcode )
        incrementLastRepeat();
    else
        addAlarm( InvalidBarcode );
}

void AlarmsTableModel::raiseMissingHoleAlarm()
{
    if( !missingHoleEnabled )
        return;

    if( ids.size() > 0 && typeids.front() == MissingHole )
        incrementLastRepeat();
    else
        addAlarm( MissingHole );
}

void AlarmsTableModel::raiseInvalidProfileAlarm()
{
    if( !invalidProfileEnabled )
        return;

    if( ids.size() > 0 && typeids.front() == InvalidProfile )
        incrementLastRepeat();
    else
        addAlarm( InvalidProfile );
}

void AlarmsTableModel::incrementLastRepeat()
{
    repeats.front()++;

    QSqlQuery q;
    q.prepare( "UPDATE alarms SET repeat=? WHERE id=?;");
    q.bindValue( 1, repeats.front() );
    q.bindValue( 2, ids.front() );
    q.exec();

    emit layoutChanged();
}

void AlarmsTableModel::addAlarm( const AlarmTypes type )
{
    QSqlQuery q;
    q.prepare( "INSERT INTO 'alarms' (alarm_types_id, repeat) VALUES(?, 1);" );
    q.bindValue( 0, (int)type );
    if( q.exec() )
    {
        ids.prepend( q.lastInsertId().toInt() );
        typeids.prepend( type );
        repeats.prepend( 1 );
        timestamps.prepend( QDateTime::currentDateTime() );
    }

    emit layoutChanged();
}

QString AlarmsTableModel::AlarmMessage( const AlarmTypes type ) const
{
    switch( type )
    {
        case BarcodeTimeout:
            return QString( "Barcode timeout" );
        case InvalidBarcode:
            return QString( "Invlid barcode" );
        case MissingHole:
            return QString( "Missing hole" );
        case InvalidProfile:
            return QString( "Invlid profile" );
        default:
            return QString();
    }
}
