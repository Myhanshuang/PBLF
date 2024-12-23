#ifndef CLICKABLEPIXMAPITEM_H
#define CLICKABLEPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QCursor>

class ClickablePixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit ClickablePixmapItem(const QPixmap& pixmap, QGraphicsItem* parent = nullptr);

    // New method to resize the pixmap
    void resizeTo(const QSize& targetSize);


signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override ;
};

#endif // CLICKABLEPIXMAPITEM_H
