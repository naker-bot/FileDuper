#include <QCoreApplication>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    QString testPath = "ftp://192.168.1.224/sdb/DokuFilme/";
    
    qInfo() << "Testing path:" << testPath;
    qInfo() << "startsWith ftp://:" << testPath.startsWith("ftp://");
    qInfo() << "length:" << testPath.length();
    qInfo() << "first 6 chars:" << testPath.left(6);
    
    if (testPath.startsWith("ftp://")) {
        qInfo() << "✅ FTP path detected correctly";
    } else {
        qInfo() << "❌ FTP path NOT detected";
    }
    
    return 0;
}