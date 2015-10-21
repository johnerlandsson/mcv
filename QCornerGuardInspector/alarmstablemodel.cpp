#include "alarmstablemodel.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

AlarmsTableModel::AlarmsTableModel()
{
//    db.addDatabase( "QSQLITE" );
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
                return messages[index.row()];
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
            messages[index.row()] = value.toString();
            break;
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

void AlarmsTableModel::loadData()
{
    QSqlQuery q;
    bool res = false;

    res = q.exec( "SELECT 'alarms'.'timestamp', 'alarm_types'.'msg', 'alarms'.'repeat', 'alarms'.'id'\n"
                  "FROM 'alarms'\n"
                  "LEFT JOIN 'alarm_types' ON 'alarms'.'alarm_types_id'='alarm_types'.'id';" );

    if( !res )
        return;

    emit layoutAboutToBeChanged();

    timestamps.clear();
    messages.clear();
    repeats.clear();
    ids.clear();

    while( q.next() )
    {
        timestamps << q.value( 0 ).toDateTime();
        messages << q.value( 1 ).toString();
        repeats << q.value( 3 ).toInt();
        ids << q.value( 4 ).toInt();
    }

    emit layoutChanged();
}
