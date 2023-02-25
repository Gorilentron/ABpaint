#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

//definimos el canvas donde se dibuja
class Canvas;


class MainWindow : public QMainWindow
{
    //declaramos la clase como un Qobject, que es el base para todos los objetos (para manejar eventos)
    Q_OBJECT

public:
    MainWindow();

protected:
    // esta función concluye el evento
    void closeEvent(QCloseEvent *event) override;

//estos son los eventos que pueden ocurrir en la app
private slots:
    void open();
    void save();
    void color();
    void width();
    void about();

private:
    //conecta las funciones con las acciones realizadas por el usuario
    void createactions();
    void createmenus();
    // comprobador de que los cambios se guarden
    bool maybesave();

    //mensaje de guardado
    bool savefile(const QByteArray &fileformat);

    //el canvas donde se dibuja
    Canvas *canvas;

    //el menu y sus opciones
    QMenu *saveasmenu;
    QMenu *filemenu;
    QMenu *optionmenu;
    QMenu *helpmenu;

    //acciones que pueden ocurrir
    QAction *openact;

    //acciones específicas al formato del archivo
    QList<QAction *> saveasact;
    QAction *exitact;
    QAction *coloract;
    QAction *widthact;
    QAction *printact;
    QAction *clearscreenact;
    QAction *aboutact;
    QAction *aboutQTact;


};
#endif // MAINWINDOW_H
