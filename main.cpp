#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //la aplicación que se crea
    QApplication app(argc, argv);
    //esto crea la ventana de la misma y la abre
    MainWindow window;
    window.show();
    //esto muestra la propia ventana
    return app.exec();
}
