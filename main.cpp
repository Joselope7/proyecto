#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << QSqlDatabase::drivers();
    return 0;
}