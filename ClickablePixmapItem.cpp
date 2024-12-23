#include "ClickablePixmapItem.h"
#include <QGraphicsSceneMouseEvent>

// Constructor implementation (if needed)
ClickablePixmapItem::ClickablePixmapItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent) {
    setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
    // Optionally, set any default properties or behaviors here
}

// Override mousePressEvent to emit the `clicked` signal
void ClickablePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();  // Emit the clicked signal when the left mouse button is pressed
    }

}

// Add a method to scale the image dynamically
void ClickablePixmapItem::resizeTo(const QSize& targetSize) {
    if (!pixmap().isNull()) {
        setPixmap(pixmap().scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
