#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class Canvas : public QWidget{
    //lo declaramos como qobjeto
    Q_OBJECT
public:
    Canvas(QWidget *parent = 0);

    //para los eventos
    bool openimage(const QString &filename);
    bool saveimage(const QString &filename, const char *fileformat);
    void setcolor(const QColor &newcolor);
    void setwidth(int newwidth);

    //ver si la imagen ha sido modificada desde el último guardado
    bool isModified() const { return modified; }
    QColor color() const { return mycolor;}
    int width() const {return mywidth;}

public slots:
    //eventos manejables
    void clearimage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //actualizar el canvas donde se dibuja
    void paintEvent(QPaintEvent *event) override;
    //nos aseguramos que el area de dibujo siga igual de tamaño
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawlineto(const QPoint &endpoint);
    void resizeimage(QImage *image, const QSize &newsize);

    //true o false en base si se han realizado cambios al guardar
    bool modified;
    // lo mismo, pero si se está dibujando
    bool scribbling;
    //se guarda el valor del tamaño y color del "lápiz"
    int mywidth;
    QColor mycolor;
    //guarda la imagen
    QImage image;

    //guarda la posición del cursor
    QPoint lastpoint;

};


#endif // CANVAS_H
