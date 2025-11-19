#include <QApplication>
#include "directorytreewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    DirectoryTreeWidget window;
    window.show();
    
    return app.exec();
}
