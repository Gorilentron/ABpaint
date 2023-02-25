#include <QtWidgets>
#include "mainwindow.h"
#include "canvas.h"

//constructor de mainwindow
MainWindow:: MainWindow(){
    //crear el canvas y hacerlo el principal
    canvas = new Canvas;
    setCentralWidget(canvas);

    //crear los menús y las acciones
    createactions();
    createmenus();

   //poner un título
    setWindowTitle(tr("Aplicacion ABpaint"));

   // darle un tamaño a la ventana
    resize(500, 500);
}

//cierre de la aplicación
void MainWindow:: closeEvent(QCloseEvent *event){
    //si cierran maybesave() devuelve true, si no se han efectuado cambios
    if (maybesave()){
        event->accept();
    }
    else {
        // en el caso de que hay cambios
        event->ignore();
    }
}

//Comprobar si la imagen ha sido cambiada y abrir un mensaje para abrir un archivo
void MainWindow :: open(){
    //comprobar si hay cambios
    if (maybesave()) {
       //hacer que el archivo salga de un mensaje, tr hace la ventana para abrir el archivo y Qdir abre el archivo
       QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),QDir:: currentPath());
    //carga la imagen del archivo
    if (!filename.isEmpty())
        canvas->openimage(filename);
    }
}

//el usuario usa "guardar como" en el menu del canvas
void MainWindow::save(){
    //acción del click del usuario
    QAction *action = qobject_cast<QAction *> (sender());
//guardar el array de bytes de los datos del usuario
    QByteArray fileformat = action->data().toByteArray();

//se pasa para almacenarlo
    savefile(fileformat);

}

//mensaje de cambio de color
void MainWindow:: color(){
    //guarda el color actual
    QColor newcolor = QColorDialog::getColor(canvas->color());
    //si el color es válido, se cambia
    if (newcolor.isValid())
        canvas->setcolor(newcolor);
}

//mensaje de cambio de tamaño
void MainWindow:: width(){
    //guardar el valor original
    bool ok;
    //definimos los valores y el texto del mensaje
    int newwidth = QInputDialog:: getInt(this, tr("Scribble"),tr ("select width:"),canvas->width(),1,50,1,&ok);

    //cambiar el tamaño
    if (ok)
        canvas->setwidth(newwidth);
}

//menu about
void MainWindow:: about(){
    //mensaje y display
    QMessageBox:: about(this,tr("about scribble"),tr("<p>Esto <b>es </b>un placeholder </p>"));

}

//definir las acciones del menu para llamar a las otras funciones
void MainWindow:: createactions(){
    //abrir menú
    openact = new QAction(tr("&Open..."),this);
   //asignar la tecla de atajo
    openact-> setShortcut(QKeySequence::Open);

   //vincular la accion a mainwindow::open()
    connect(openact,SIGNAL(triggered()),this,SLOT(open()));

   //lista de los formatos aceptados y hacer imagenes a archivos con QImageWriter
    foreach(QByteArray format, QImageWriter:: supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        //una accion para cada uno de los formatos soportados
        QAction *action = new QAction(text,this);
        // administrar una acción para cada formato
        action->setData(format);
        //cuando se ejecute, llamar a la función save
        connect(action,SIGNAL(triggered()),this,SLOT(save()));
        //unir cada opción de formato al menu en "guardar como"
        saveasact.append(action);
    }


//crear la accion print y añadirla a mainwindow
    printact = new QAction(tr("&Print..."),this);
    connect(printact, SIGNAL(triggered()),canvas,SLOT(print()));


//funcion salir y juntarla a mainwindow
    exitact = new QAction(tr("&Exit..."),this);
    exitact->setShortcuts(QKeySequence::Quit);
    connect(exitact, SIGNAL(triggered()),this, SLOT(close()));


//accion del color del "lapiz"
    coloract = new QAction(tr("&Color..."),this);
    connect(coloract,SIGNAL(triggered()),this,SLOT(color()));

//accion del tamaño del "lapiz"
    widthact = new QAction(tr("Pen &Width..."),this);
    connect(widthact, SIGNAL(triggered()),this, SLOT( width()));

//limpiar el canvas
    clearscreenact = new QAction(tr("&Clear screen..."),this);
    clearscreenact->setShortcut(tr("Ctrl+L"));
    connect(clearscreenact,SIGNAL(triggered()),canvas,SLOT (clearimage()));

//funcion about
    aboutact = new QAction(tr("&About..."),this);
    connect (aboutact, SIGNAL (triggered()),this,SLOT (about()));

//funcion aboutQT (todas estas funciones se están añadiendo al MainWindow (lo que se ha creado con el constructor))
    aboutQTact = new QAction(tr("&AboutQT..."),this);
    connect (aboutQTact, SIGNAL(triggered()),qApp,SLOT (aboutQT()));

}

//Vamos a crear la barra del menu
void MainWindow:: createmenus(){
    //creamos el guardar como y la lista de formatos
    saveasmenu = new QMenu (tr("&Save as"),this);
    foreach (QAction *action,saveasact)
        saveasmenu->addAction(action);

    //añadir todas las acciones al archivo
    filemenu = new QMenu(tr("&File"),this);
    filemenu-> addAction(openact);
    filemenu-> addMenu(saveasmenu);
    filemenu-> addAction(printact);
    filemenu-> addSeparator();
    filemenu-> addAction(exitact);

    //añadimos las acciones del menu opciones
    optionmenu = new QMenu(tr("&Options"),this);
    optionmenu-> addAction(coloract);
    optionmenu-> addAction(widthact);
    optionmenu-> addSeparator();
    optionmenu-> addAction(clearscreenact);

    //añadir las acciones del menu help
    helpmenu = new QMenu(tr("&Help"),this);
    helpmenu -> addAction (aboutact);
    helpmenu-> addAction(aboutQTact);

    //añadir los items del menu a la barra
    menuBar()->addMenu(filemenu);
    menuBar()->addMenu(helpmenu);
    menuBar()->addMenu(optionmenu);

}

bool MainWindow:: maybesave(){
    //Comprobar si hay cambios desde el último guardado
    if (canvas->isModified()){
        QMessageBox::StandardButton ret;

        //añadimos el título y los botones
        ret= QMessageBox::warning(this,tr("canvas"),tr("the image has been altered.\n " "Want to save your changes?"),
                                  QMessageBox::Save|
                                  QMessageBox::Discard
                                  |QMessageBox::Cancel);

        // guardar el archivo
        if (ret == QMessageBox::Save){
            return savefile("png");
        }

        //cancelar
        else if (ret == QMessageBox::Cancel){
            return false;
        }

    }
    return true;
}

bool MainWindow::savefile(const QByteArray &fileformat){
    //definir el path, el nombre y el tipo del documento
    QString initialpath = QDir::currentPath()+"/untitled."+fileformat;

    //obtener el archivo seleccionado y añadir los formatos y extensiones adecuadas
    QString filename = QFileDialog:: getSaveFileName(this,tr("Save as"), initialpath, tr("%1 Files (*.%2);;All files (*)")

    .arg(QString::fromLatin1(fileformat.toUpper()))

    .arg(QString::fromLatin1(fileformat)));

    //en el caso de no haber archivo
    if (filename.isEmpty()){
        return false;
    }   else{

        //se llama al archivo para guardarlo
        return canvas->saveimage(filename,fileformat.constData());

    }

}


