#include <QtWidgets>
#include "canvas.h"
Canvas::Canvas(QWidget *parent)
    : QWidget(parent){
    //ancla el widget a la esquina superior izquierda
    setAttribute(Qt::WA_StaticContents);

    //determina los estándares para las variables monitorizadas
    modified = false;
    scribbling = false;
    mywidth = 1;
    mycolor = Qt::black;

}

//cargar la imagen y colocarla en el widget
bool Canvas::openimage(const QString &filename){

    //mantiene la imagen
    QImage loadedimage;

    //en caso de no haber cargado la imagen
    if (!loadedimage.load(filename))
        return false;


    QSize newsize = loadedimage.size().expandedTo(size());
    resizeimage(&loadedimage, newsize);
    image = loadedimage;
    modified = false;
    update();
    return true;

}


//Guardar la imagen actual
bool Canvas:: saveimage(const QString &filename, const char *fileformat){
    //para mostrar la imagen
    QImage visibleimage = image;
    resizeimage(&visibleimage, size());

    if (visibleimage.save(filename,fileformat)){
        modified = false;
        return true;

    }   else{
        return false;
    }
}

//cambiar el color
void Canvas:: setcolor(const QColor &newcolor){
    mycolor = newcolor;
}
//cambia el tamaño
void Canvas:: setwidth(int newwidth){
    mycolor = newwidth;
}
//pinta el area de la imagen con blanco (borra)

void Canvas:: clearimage(){
    image.fill(qRgb(255,255,255));
    modified = true;
    update ();
}

//comprobación de click del ratón, en caso de que sea el izquierdo llama a la función de dibujar para que dibuje en base a la última posición del cursor
void Canvas::mousePressEvent(QMouseEvent *event){
    if(event->button()== Qt::LeftButton){
        lastpoint = event->pos();
        scribbling = true;
    }
}




// cuando el ratón se mueve, deplazarlo y se mantiene pulsado el click, dibuja una linea desde la posición "a" hasta "b"
void Canvas:: mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) &&scribbling)
        drawlineto(event->pos());
}

//Cuando el botón se suelta, se deja de escribir
void Canvas:: mouseReleaseEvent(QMouseEvent *event){
    if(event->button()== Qt::LeftButton && scribbling){
        drawlineto(event-> pos());
        scribbling = false;
    }
}

//usamos Qpaint y QpaintEvent para actualizar los widgets
void Canvas:: paintEvent(QPaintEvent *event){
    QPainter painter(this);

    //devuelve el rectángulo que tiene que ser actualizado
    QRect dirtyrect = event->rect();

    //dibuja el rectángulo donde se actualiza la imagen
    painter.drawImage(dirtyrect,image,dirtyrect);
}

//Redimensionar la imagen
void Canvas:: resizeEvent(QResizeEvent *event){
    if (width() > image.width() || height() > image.height()){
        int newWidht = qMax(width() +128, image.width());
        int newHeight = qMax(height()+128, image.height());

        resizeimage(&image, QSize(newWidht,newHeight));
        update();
    }
    QWidget:: resizeEvent(event);
}

void Canvas:: drawlineto(const QPoint &endpoint){
    //para poder dibujar en el widget
    QPainter painter(&image);

    //cambiar la configuración actual del "lápiz"
    painter.setPen(QPen(mycolor,mywidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    //Dibujar la linea desde el último punto registrado hasta el actual
    painter.drawLine(lastpoint, endpoint);

    //marcar que la imagen no ha sido guardada
    modified = true;

    int rad = (mywidth / 2)+2;

    //llamada para actualizar el rectángulo donde se ha dibujado
    update(QRect(lastpoint,endpoint).normalized().adjusted(-rad,-rad,+rad,+rad));


    //actualizar la última posición en la que dibujamos
    lastpoint = endpoint;
}

//cuando la app es redimensionada, crear una nueva imagen usando los cambios realizados a esta
void Canvas:: resizeimage(QImage *image, const QSize &newsize){
    //se comprueba si no hay que redibujar la imagen
    if(image->size()==newsize)
        return;
    //se crea una nueva imagen y se rellena con blanco
    QImage newimage(newsize, QImage::Format_RGB32);
    newimage.fill(qRgb(255,255,255));

    //se dibuja la imagen
    QPainter painter(&newimage);
    painter.drawImage(QPoint(0,0),*image);
    *image = newimage;


}


//muestra la imagen
void Canvas::print(){
    //se comprueba la disponibilidad de la imagen
}

