#include <QCoreApplication>
#include <iostream>
#include "database.h"
#include <QThread>
#include <QSqlQuery>
#include <QDebug>

using namespace std;

enum Language {en = 1, hu = 2};
enum LabelType {title = 1, definition = 2, longdefinition = 3, fullyspecifiedname = 4, inclusion = 5, exclusion = 6, synonym = 7, narrowerterm = 8, simplified = 9};

int main()
{
    cout << "The backend is running." << endl;

    Database db;
    QString err;
    db.connectDatabase(&err);
    cout << err.toStdString() << endl;

    return 0;
}
